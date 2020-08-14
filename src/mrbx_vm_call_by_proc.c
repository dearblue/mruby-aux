#include <mruby-aux/vmext.h>
#include <mruby-aux/proc.h>
#include <mruby-aux/compat/mruby.h>

mrb_value
mrbx_vm_call_by_proc(mrb_state *mrb, struct RClass *tc, struct RProc *p,
    mrb_sym mid, mrb_value recv, int argc, const mrb_value argv[], mrb_value block)
{
  mrb_method_t m = mrbx_method_wrap_proc(p, MRB_ARGS_ANY());
  return mrbx_vm_call_by_method(mrb, tc, m, mid, recv, argc, argv, block);
}
