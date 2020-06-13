#include <mruby.h>
#include <mruby-aux/vmext.h>

mrb_callinfo *
mrbx_vm_cipush(mrb_state *mrb, const mrb_code *pc, int push_stacks, int acc,
    struct RClass *target_class, mrb_sym mid, int argc)
{
  struct mrb_context *c = mrb->c;
  mrb_callinfo *ci = c->ci;

  const mrb_callinfo ci_init = {
    mid,
    NULL,
    c->stack,
#if MRUBY_RELEASE_NO < 20000
    0,
#endif
    ci->ridx,
#if MRUBY_RELEASE_NO < 10300
    ci->eidx,
#else
    mrb->c->eidx,
#endif
    NULL,
    (mrb_code *)pc,
    NULL,
    argc,
    acc,
    target_class
  };

  mrbx_vm_ciextend(mrb, 1);

  ci = ++ c->ci;
  *ci = ci_init;
  c->stack += push_stacks;

  return ci;
}
