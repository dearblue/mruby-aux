#ifndef MRUBY_AUX_VMEXT_H
#define MRUBY_AUX_VMEXT_H 1

#include <mruby.h>
#include "compat/proc.h"

/*
 * `mrb->c->ci` をもとに親クラスのメソッドを呼びます。
 */
mrb_value mrbx_vm_call_super(mrb_state *mrb, mrb_value recv, int argc, const mrb_value argv[], mrb_value block);

/*
 * 引数 `m` をメソッド呼び出しします。
 */
mrb_value mrbx_vm_call_by_method(mrb_state *mrb, struct RClass *tc, mrb_method_t m,
    mrb_sym mid, mrb_value recv, int argc, const mrb_value argv[], mrb_value block);

/*
 * 引数 `p` をメソッド呼び出しします。
 */
mrb_value mrbx_vm_call_by_proc(mrb_state *mrb, struct RClass *tc, struct RProc *p,
    mrb_sym mid, mrb_value recv, int argc, const mrb_value argv[], mrb_value block);

/*
 * 使い方に注意！
 *
 * ***基本的にこの関数を呼んだら、あとの処理を行うことは出来ません。***
 * ***関数の戻り値をそのまま `return` して下さい。***
 *
 * ```c
 * return mrbx_vm_call_interchange(mrb, ...);
 * ```
 *
 * この関数は現在呼び出し中のコールスタックをすり替えて、
 * あたかも proc を呼び出したかのように観測者を騙します。
 */
mrb_value mrbx_vm_call_interchange(mrb_state *mrb, struct RClass *target_class, mrb_method_t m,
    mrb_value self, mrb_sym mid, mrb_int argc, const mrb_value argv[], mrb_value block);

/*
 * `mrb->c->ci` に追加します。
 */
mrb_callinfo *mrbx_vm_cipush(mrb_state *mrb, int push_stacks, int16_t cci,
    struct RClass *target_class, struct RProc *proc, mrb_sym mid, int16_t argc);

/*
 * 現在の`mrb->c->ci` を削除します。
 */
mrb_callinfo *mrbx_vm_cipop(mrb_state *mrb);

/*
 * `mrb->c->ci` を `n` 個以上の空きがあるように拡張します。
 */
mrb_callinfo *mrbx_vm_ciextend(mrb_state *mrb, size_t n);

/*
 * スタックを拡張します。
 * `mrb_stack_extend()` のラッパー関数ですが、戻り値として現在のスタックポインタを返します。
 */
mrb_value *mrbx_vm_stack_extend(mrb_state *mrb, mrb_int room);

/*
 * 指定された `ci` に対してメソッド引数を設定します。
 *
 * `mid` は 0 以外であれば、`argv` の前に追加されます。この機能は `Object#send` や `Object#method_missing` などで使うことが出来ます。
 *
 * 戻り値は専有したスタックの個数を意味します。`mrb_vm_run()` の第四引数 `stack_keep` としてそのまま渡すことを意図しています。
 */
int mrbx_vm_set_args(mrb_state *mrb, mrb_callinfo *ci, mrb_value recv, int argc, const mrb_value argv[], mrb_value block, mrb_sym mid);

/**
 * 現在の `ci` の引数から先頭を取り除いて戻り値とします。
 * 引数が空であれば `mrb_undef_value()` を返します。
 */
mrb_value mrbx_vm_shift_args(mrb_state *mrb);

/**
 * 現在の `ci` の引数の先頭に与えられたオブジェクトを追加します。
 *
 * 戻り値は専有したスタックの個数を意味します。`mrb_vm_run()` の第四引数 `stack_keep` としてそのまま渡すことを意図しています。
 */
int mrbx_vm_unshift_args(mrb_state *mrb, mrb_value obj);

/**
 * 現在の呼び出しレベルのスタックポインタを取得します。
 */
static inline mrb_value *mrbx_vm_top_stacks(const struct mrb_context *c);

/**
 * コールスタックを遡ってスタックポインタを取得します。
 * 0 を指定する場合、結果は `mrbx_vm_top_stacks()` と同じです。
 */
static inline mrb_value *mrbx_vm_stacks(const struct mrb_context *c, size_t backref);

/**
 *
 */
static inline mrb_value *mrbx_vm_stacks_nocheck(const struct mrb_context *c, size_t backref);

#ifdef MRUBY_AUX_INTERNALS
mrb_value mrbx_vm_intercall(mrb_state *mrb, mrb_callinfo *ci, struct RProc *proc, mrb_func_t cfunc, mrb_value recv, int keeps);
#endif


static inline mrb_value *
mrbx_vm_top_stacks(const struct mrb_context *c)
{
#if MRUBY_RELEASE_NO >= 30000
  if (c->ci) {
    return c->ci->stack;
  } else {
    return c->stbase;
  }
#else
  return c->stack;
#endif
}

static inline mrb_value *
mrbx_vm_stacks(const struct mrb_context *c, size_t backref)
{
  if (backref == 0) {
    return mrbx_vm_top_stacks(c);
  }

  ptrdiff_t cinum = c->ci - c->cibase;
  if (cinum <= 0 || (size_t)cinum < backref) {
    return NULL;
  }

  return mrbx_vm_stacks_nocheck(c, backref);
}

static inline mrb_value *
mrbx_vm_stacks_nocheck(const struct mrb_context *c, size_t backref)
{
  mrb_assert(backref > 0);
  mrb_assert(c->cibase != NULL);
  mrb_assert(c->ci - c->cibase > 0);
  mrb_assert((size_t)(c->ci - c->cibase) < backref);

#if MRUBY_RELEASE_NO < 30000
  return c->ci[-(backref - 1)].stackent;
#else
  return c->ci[-backref].stack;
#endif
}

#endif /* MRUBY_AUX_VMEXT_H */
