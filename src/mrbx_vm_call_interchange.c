#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/irep.h>
#include <mruby/proc.h>
#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/mruby.h>

#ifndef MRB_FUNCALL_ARGC_MAX
# define MRB_FUNCALL_ARGC_MAX 16
#endif

#define STR_METHOD_MISSING      "method_missing"
#define ID_METHOD_MISSING       mrb_intern_lit(mrb, STR_METHOD_MISSING)

static mrb_sym
resolve_method_missing(mrb_state *mrb, struct RClass **target_class, struct RProc **proc, mrb_func_t *cfunc)
{
  mrb_sym mid = ID_METHOD_MISSING;
  mrb_method_t me = mrb_method_search_vm(mrb, target_class, mid);

  if (MRB_METHOD_UNDEF_P(me)) {
    mrb_raise(mrb, E_NOMETHOD_ERROR, "undefined method '" STR_METHOD_MISSING "'");
  }

  if (MRB_METHOD_NOARG_P(me)) {
    mrb_argnum_error(mrb, -1, 0, 0);
  }

  if (MRB_METHOD_PROC_P(me)) {
    *cfunc = NULL;
    *proc = MRB_METHOD_PROC(me);
  } else {
    *cfunc = MRB_METHOD_FUNC(me);
    *proc = NULL;
  }

  return mid;
}

mrb_value
mrbx_vm_call_interchange(mrb_state *mrb, struct RClass *target_class, struct RProc *proc,
    mrb_value self, mrb_sym mid, mrb_int argc, const mrb_value argv[], mrb_value block)
{
  mrb_func_t cfunc;
  mrb_sym original_mid = 0;

  if (!proc) {
    original_mid = mid;
    mid = resolve_method_missing(mrb, &target_class, &proc, &cfunc);
  } else {
    cfunc = NULL;
  }

  /*
   * 1. 呼び出し先が C 関数である
   *    => 現在の callinfo を書き換えて、C 関数を呼ぶ
   * 2. 呼び出し元が C 関数である
   *    => 現在の callinfo を書き換えて、`mrb_vm_exec()` を呼ぶ
   * 3. 呼び出し元が ruby 空間である
   *    => callinfo を1つ積み増しして、戻った時に目的となるメソッドが実行されるようにする
   */

  mrb_callinfo *ci = mrb->c->ci;
  ci->mid = mid;
  ci->proc = cfunc ? NULL : proc;
  ci->env = NULL;
  ci->target_class = target_class;
  int keeps = mrbx_vm_set_args(mrb, ci, self, argc, argv, block, original_mid);

  if (cfunc) {
call_cfunc: ;
    int ai = mrb_gc_arena_save(mrb);
    mrb_value ret = cfunc(mrb, self);
    mrb_gc_arena_restore(mrb, ai);
    mrb_gc_protect(mrb, ret);
    return ret;
  } else if (MRB_PROC_CFUNC_P(proc)) {
    cfunc = MRBX_PROC_CFUNC(proc);
    goto call_cfunc;
  } else if (ci->acc < 0 || ci == mrb->c->cibase || ci[-1].proc == NULL || MRB_PROC_CFUNC_P(ci[-1].proc)) {
    ptrdiff_t idx = mrb->c->ci - mrb->c->cibase;
    ci->acc = -1; /* ACC_SKIP */
    mrb_value ret = mrb_vm_run(mrb, proc, self, keeps);
    mrb->c->ci = mrb->c->cibase + idx;
    return ret;
  } else {
    mrbx_vm_cipush(mrb, proc->body.irep->iseq, 0, 0, NULL, 0, 0);
    return self;
  }
}
