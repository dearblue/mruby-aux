#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/mruby.h>
#include <mruby-aux/compat/proc.h>
#include <mruby-aux/proc.h>

mrb_value
mrbx_vm_call_by_method(mrb_state *mrb, struct RClass *tc, mrb_method_t m,
    mrb_sym mid, mrb_value recv, int argc, const mrb_value argv[], mrb_value block)
{
  mrb_func_t cfunc;
  struct RProc *proc;

  mrb_assert(!MRB_METHOD_UNDEF_P(m));
  mrbx_method_extract(mrb, m, argc, &proc, &cfunc);

  int ai = mrb_gc_arena_save(mrb);
  mrb_callinfo *ci = mrbx_vm_cipush(mrb, 0, -2 /* ACC_DIRECT */, tc, proc, mid, 0 /* dummy for argc */);
  int keeps = mrbx_vm_set_args(mrb, ci, recv, argc, argv, block, 0);

  mrb_value ret;
  if (cfunc) {
    ret = cfunc(mrb, recv);
    mrbx_vm_cipop(mrb);
  } else {
    ci->acc = -1; /* ACC_SKIP */
    ret = mrb_vm_run(mrb, proc, recv, keeps);
  }

  mrb_gc_arena_restore(mrb, ai);
  mrb_gc_protect(mrb, ret);

  return ret;
}
