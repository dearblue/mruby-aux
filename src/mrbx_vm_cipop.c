#include <mruby.h>
#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/proc.h>
#include <mruby-aux/version.h>

#if MRBX_VERSION_OLDER_OR_30100_P
# define mrb_env_unshare(M, E, R) (mrb_env_unshare(M, E), TRUE)
#endif

mrb_callinfo *
mrbx_vm_cipop(mrb_state *mrb)
{
  struct mrb_context *c = mrb->c;
  struct REnv *e = mrb_vm_ci_env(c->ci);

  if (e) {
    mrb_vm_ci_env_set(c->ci, NULL);
    mrb_env_unshare(mrb, e, TRUE);
  }
#if MRUBY_RELEASE_NO < 30000
  c->stack = c->ci->stackent;
#endif
  c->ci--;

  return c->ci;
}
