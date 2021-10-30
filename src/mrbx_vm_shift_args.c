#include <mruby-aux/vmext.h>
#include <mruby/array.h>
#include "vm_internals.h"

mrb_value
mrbx_vm_shift_args(mrb_state *mrb)
{
  mrb_value *argp = mrbx_vm_top_stacks(mrb->c) + 1 /* skip self */;

  if (CI_ARGS_PACKED_P(mrb->c->ci)) {
    if (RARRAY_LEN(*argp) > 0) {
      return mrb_ary_shift(mrb, *argp);
    }
  } else if (CI_ARGC(mrb->c->ci) > 0) {
    mrb_value obj = argp[0];
    memmove(&argp[0], &argp[1], sizeof(mrb_value) * (CI_ARGC(mrb->c->ci) + 1 /* block */ - 1 /* first value */));
    CI_ARGC(mrb->c->ci)--;
    return obj;
  }

  return mrb_undef_value();
}
