#include <mruby-aux/vmext.h>
#include <mruby/array.h>

mrb_value
mrbx_vm_shift_args(mrb_state *mrb)
{
  mrb_value *argp = mrb->c->stack + 1 /* skip self */;

  if (mrb->c->ci->argc < 0) {
    if (RARRAY_LEN(*argp) > 0) {
      return mrb_ary_shift(mrb, *argp);
    }
  } else if (mrb->c->ci->argc > 0) {
    mrb_value obj = argp[0];
    memmove(&argp[0], &argp[1], sizeof(mrb_value) * (mrb->c->ci->argc + 1 /* block */ - 1 /* first value */));
    mrb->c->ci->argc--;
    return obj;
  }

  return mrb_undef_value();
}
