#include <mruby.h>
#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/proc.h>

mrb_callinfo *
mrbx_vm_cipop(mrb_state *mrb)
{
  struct mrb_context *c = mrb->c;

  struct REnv *e = mrb_vm_ci_env(c->ci);
  if (e) {
    mrb_env_unshare(mrb, e);
  }
#if MRUBY_RELEASE_NO < 30000
  c->stack = c->ci->stackent;
#endif
  c->ci--;

  return c->ci;
}
