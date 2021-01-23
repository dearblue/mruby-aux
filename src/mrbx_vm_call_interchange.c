#define MRUBY_AUX_INTERNALS 1
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/irep.h>
#include <mruby/proc.h>
#include <mruby-aux/proc.h>
#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/mruby.h>

mrb_value
mrbx_vm_call_interchange(mrb_state *mrb, struct RClass *target_class, mrb_method_t m,
    mrb_value self, mrb_sym mid, mrb_int argc, const mrb_value argv[], mrb_value block)
{
  mrb_func_t cfunc;
  struct RProc *proc;
  mrb_sym original_mid;

  if (MRB_METHOD_UNDEF_P(m)) {
    original_mid = mid;
    mid = mrbx_resolve_method_missing(mrb, &target_class, &proc, &cfunc);
  } else {
    original_mid = 0;
    mrbx_method_extract(mrb, m, argc, &proc, &cfunc);
  }

  mrb_callinfo *ci = mrb->c->ci;
  ci->mid = mid;
  ci->proc = proc;
#if MRUBY_RELEASE_NO < 30000
  ci->env = NULL;
  ci->target_class = target_class;
#else
  ci->u.target_class = target_class;
#endif
  int keeps = mrbx_vm_set_args(mrb, ci, self, argc, argv, block, original_mid);
  return mrbx_vm_intercall(mrb, ci, proc, cfunc, self, keeps);
}
