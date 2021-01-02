#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/mruby.h>
#include <mruby/array.h>

int
mrbx_vm_unshift_args(mrb_state *mrb, mrb_value obj)
{
  mrb_value *argp = mrb->c->stack + 1 /* skip self */;

  if (mrb->c->ci->argc < 0) {
    mrb_ary_unshift(mrb, *argp, obj);
    return 3;
  } else {
    mrb_stack_extend(mrb, mrb->c->ci->argc + 1 /* shift value */ + 1 /* self */ + 1 /* block */);
    argp = mrb->c->stack + 1 /* skip self */;
    memmove(&argp[1], &argp[0], sizeof(mrb_value) * (mrb->c->ci->argc + 1 /* block */));
    argp[0] = obj;
    mrb->c->ci->argc++;
    return mrb->c->ci->argc + 1 /* self */ + 1 /* block */;
  }
}
