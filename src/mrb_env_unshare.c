#include <mruby/version.h>

#if MRUBY_RELEASE_NO < 10300

#include <mruby.h>
#include <mruby/proc.h>

static mrb_value *
detach_stack(mrb_state *mrb, const mrb_value *src, size_t num)
{
  if (num < 1) {
    return NULL;
  }

  mrb_value *dest = (mrb_value *)mrb_malloc(mrb, sizeof(mrb_value) * num);
  while (num -- > 0) {
    dest[num] = src[num];
  }

  return dest;
}

void
mrb_env_unshare(mrb_state *mrb, struct REnv *e)
{
  if (e && MRB_ENV_STACK_SHARED_P(e)) {
    e->stack = detach_stack(mrb, e->stack, MRB_ENV_STACK_LEN(e));
    MRB_ENV_UNSHARE_STACK(e);
    mrb_write_barrier(mrb, (struct RBasic *)e);
  }
}

#endif /* MRUBY_RELEASE_NO < 10300 */
