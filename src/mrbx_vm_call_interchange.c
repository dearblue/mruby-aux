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
mrbx_vm_call_interchange(mrb_state *mrb, struct RClass *target_class, struct RProc *proc,
    mrb_value self, mrb_sym mid, mrb_int argc, const mrb_value argv[], mrb_value block)
{
  mrb_func_t cfunc;
  mrb_sym original_mid = 0;

  if (!proc) {
    original_mid = mid;
    mid = mrbx_resolve_method_missing(mrb, &target_class, &proc, &cfunc);
  } else if (MRB_PROC_CFUNC_P(proc)) {
    cfunc = MRBX_PROC_CFUNC(proc);
  } else {
    cfunc = NULL;
  }

  mrb_callinfo *ci = mrb->c->ci;
  ci->mid = mid;
  ci->proc = proc;
  ci->env = NULL;
  ci->target_class = target_class;
  int keeps = mrbx_vm_set_args(mrb, ci, self, argc, argv, block, original_mid);
  return mrbx_vm_intercall(mrb, ci, proc, cfunc, self, keeps);
}
