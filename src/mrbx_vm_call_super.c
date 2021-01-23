#include <mruby-aux/vmext.h>
#include <mruby/class.h>
#include <mruby-aux/proc.h>

mrb_value
mrbx_vm_call_super(mrb_state *mrb, mrb_value recv, int argc, const mrb_value argv[], mrb_value block)
{
  mrb_callinfo *ci = mrb->c->ci;
  mrb_sym mid = mrb_get_mid(mrb);

  if (mid < 1) {
nomethod_err:
    mrb_raise(mrb, E_NOMETHOD_ERROR,
              "super: no superclass method");
  }

  struct RClass *tc = mrb_vm_ci_target_class(ci);
  if (tc == NULL) { goto nomethod_err; }
  tc = tc->super;
  if (tc == NULL) { goto nomethod_err; }
  mrb_method_t m = mrb_method_search_vm(mrb, &tc, mid);
  if (MRB_METHOD_UNDEF_P(m) || tc == NULL) { goto nomethod_err; }

  return mrbx_vm_call_by_method(mrb, tc, m, mid, recv, argc, argv, block);
}
