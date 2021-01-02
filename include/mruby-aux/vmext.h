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
mrb_callinfo *mrbx_vm_cipush(mrb_state *mrb, const mrb_code *pc, int push_stacks, int acc,
    struct RClass *target_class, struct RProc *proc, mrb_sym mid, int argc);

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

#ifdef MRUBY_AUX_INTERNALS
mrb_value mrbx_vm_intercall(mrb_state *mrb, mrb_callinfo *ci, struct RProc *proc, mrb_func_t cfunc, mrb_value recv, int keeps);
#endif

#endif /* MRUBY_AUX_VMEXT_H */
