#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/mruby.h>
#include <mruby/array.h>
#include "vm_internals.h"

int
mrbx_vm_unshift_args(mrb_state *mrb, mrb_value obj)
{
  mrb_value *argp = mrbx_vm_top_stacks(mrb->c) + 1 /* skip self */;

  if (CI_ARGS_PACKED_P(mrb->c->ci)) {
    mrb_ary_unshift(mrb, *argp, obj);
    return 3;
  } else if (CI_ARGC(mrb->c->ci) < ARGS_FORCE_PACK_THRESHOLD) {
    mrb_stack_extend(mrb, CI_ARGC(mrb->c->ci) + CI_ARGS_KW_SPACE(mrb->c->ci) + 1 /* shift value */ + 1 /* self */ + 1 /* block */);
    argp = mrbx_vm_top_stacks(mrb->c) + 1 /* skip self */;
    memmove(&argp[1], &argp[0], sizeof(mrb_value) * (CI_ARGC(mrb->c->ci) + CI_ARGS_KW_SPACE(mrb->c->ci) + 1 /* block */));
    argp[0] = obj;
    CI_ARGC(mrb->c->ci)++;
    return CI_ARGC(mrb->c->ci) + CI_ARGS_KW_SPACE(mrb->c->ci) + 1 /* self */ + 1 /* block */;
  } else {
    mrb_value splat = mrb_ary_new_from_values(mrb, CI_ARGC(mrb->c->ci), argp);
    memmove(&argp[1], &argp[CI_ARGC(mrb->c->ci)], sizeof(mrb_value) * (CI_ARGS_KW_SPACE(mrb->c->ci) + 1 /* block */));
    argp[0] = splat;
    mrb_ary_unshift(mrb, splat, obj);
    return 3;
  }
}
