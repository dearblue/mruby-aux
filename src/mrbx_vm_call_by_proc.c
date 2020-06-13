#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/mruby.h>
#include <mruby-aux/compat/proc.h>

mrb_value
mrbx_vm_call_by_proc(mrb_state *mrb, struct RClass *tc, struct RProc *p,
    mrb_sym mid, mrb_value recv, int argc, const mrb_value argv[], mrb_value block)
{
  mrb_method_t m;
  MRB_METHOD_FROM_PROC(m, p);
  return mrbx_vm_call_by_method(mrb, tc, m, mid, recv, argc, argv, block);
}
