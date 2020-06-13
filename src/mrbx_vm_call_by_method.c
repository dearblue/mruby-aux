#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/mruby.h>
#include <mruby-aux/compat/proc.h>

mrb_value
mrbx_vm_call_by_method(mrb_state *mrb, struct RClass *tc, mrb_method_t m,
    mrb_sym mid, mrb_value recv, int argc, const mrb_value argv[], mrb_value block)
{
  int ai = mrb_gc_arena_save(mrb);

  mrb_assert(!MRB_METHOD_UNDEF_P(m));

  mrb_callinfo *ci = mrbx_vm_cipush(mrb, NULL, 0, -2 /* ACC_DIRECT */, tc, mid, 0 /* dummy for argc */);
  int keeps = mrbx_vm_set_args(mrb, ci, recv, argc, argv, block, 0);
  mrb_func_t cfunc;

  mrb_value ret;
  if (MRB_METHOD_FUNC_P(m)) {
    cfunc = MRB_METHOD_FUNC(m);
call_cfunc:
    mrb_assert(cfunc);
    ret = cfunc(mrb, recv);
    mrbx_vm_cipop(mrb);
  } else {
    struct RProc *p = MRB_METHOD_PROC(m);
    mrb_assert(p != NULL);

    if (MRB_PROC_CFUNC_P(p)) {
      ci->proc = p;
      cfunc = MRBX_PROC_CFUNC(p);
      goto call_cfunc;
    } else {
      ci->acc = -1; /* ACC_SKIP */
      ret = mrb_vm_run(mrb, p, recv, keeps);
    }
  }

  mrb_gc_arena_restore(mrb, ai);
  mrb_gc_protect(mrb, ret);

  return ret;
}
