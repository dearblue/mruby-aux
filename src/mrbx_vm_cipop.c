#include <mruby.h>
#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/proc.h>

mrb_callinfo *
mrbx_vm_cipop(mrb_state *mrb)
{
  struct mrb_context *c = mrb->c;

  if (c->ci->env) {
    mrb_env_unshare(mrb, c->ci->env);
  }
  c->stack = c->ci->stackent;
  c->ci --;

  return c->ci;
}
