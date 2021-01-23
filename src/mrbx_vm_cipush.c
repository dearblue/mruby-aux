#include <mruby.h>
#include <mruby-aux/vmext.h>
#include <mruby/error.h>

mrb_callinfo *
mrbx_vm_cipush(mrb_state *mrb, int push_stacks, int16_t acc,
    struct RClass *target_class, struct RProc *proc, mrb_sym mid, int16_t argc)
{
  struct mrb_context *c = mrb->c;
  mrb_callinfo *ci = c->ci;

  const mrb_callinfo ci_init = {
#if MRUBY_RELEASE_NO >= 30000
    mid,
    argc, // int16_t argc
    acc, // int16_t acc
    proc,
    ci->stack + push_stacks, // mrb_value *stack
    NULL, // const mrb_code *pc
    { (struct REnv *)target_class }, // union { struct REnv *env }
#else /* MRUBY_RELEASE_NO < 30000 */
    mid,
    proc,
    c->stack, // mrb_value *stackent
#if MRUBY_RELEASE_NO < 20000
    0, // int nregs
#endif
    ci->ridx, // int ridx
#if MRUBY_RELEASE_NO < 10300
    ci->eidx, // int eidx
#else
    mrb->c->eidx, // int epos
#endif
    NULL, // struct REnv *env
    NULL, // const mrb_code *pc
    NULL, // const mrb_code *err
    argc, // int argc
    acc, // int acc
    target_class, // struct RClass *target_class
#endif /* MRUBY_RELEASE_NO */
  };

  mrbx_vm_ciextend(mrb, 1);

  ci = ++c->ci;
  *ci = ci_init;
#if MRUBY_RELEASE_NO < 30000
  c->stack += push_stacks;
#endif

  return ci;
}
