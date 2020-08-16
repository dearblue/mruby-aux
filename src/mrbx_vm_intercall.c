#define MRUBY_AUX_INTERNALS 1
#include <mruby-aux/vmext.h>

mrb_value
mrbx_vm_intercall(mrb_state *mrb, mrb_callinfo *ci, struct RProc *proc, mrb_func_t cfunc, mrb_value recv, int keeps)
{
  /*
   * 1. 呼び出し先が C 関数である
   *    => C 関数を呼ぶ
   * 2. 呼び出し元が C 関数である
   *    => `mrb_vm_exec()` を呼ぶ
   * 3. 呼び出し元が ruby 空間である
   *    => callinfo を1つ積み増しして、戻った時に目的となるメソッドが実行されるようにする
   */

  ci->env = NULL;

  if (cfunc) {
    int ai = mrb_gc_arena_save(mrb);
    ci->acc = -2; /* ACC_DIRECT */
    mrb_value ret = cfunc(mrb, recv);
    mrb_gc_arena_restore(mrb, ai);
    mrb_gc_protect(mrb, ret);
    return ret;
  } else if (ci->acc < 0 || ci == mrb->c->cibase || ci[-1].proc == NULL || MRB_PROC_CFUNC_P(ci[-1].proc)) {
    ptrdiff_t idx = mrb->c->ci - mrb->c->cibase;
    ci->acc = -1; /* ACC_SKIP */
    mrb_value ret = mrb_vm_run(mrb, proc, recv, keeps);
    mrb->c->ci = mrb->c->cibase + idx;
    return ret;
  } else {
    mrbx_vm_cipush(mrb, proc->body.irep->iseq, 0, 0, NULL, NULL, 0, 0);
    return recv;
  }
}
