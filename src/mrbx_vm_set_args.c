#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/proc.h>
#include <mruby/array.h>

#ifndef MRB_FUNCALL_ARGC_MAX
# define MRB_FUNCALL_ARGC_MAX 16
#endif

int
mrbx_vm_set_args(mrb_state *mrb, mrb_callinfo *ci, mrb_value recv, int argc, const mrb_value argv[], mrb_value block, mrb_sym mid)
{
  int argc_mod = argc + (mid != 0 ? 1 : 0);
  mrb_value *st = mrbx_vm_stack_extend(mrb, (argc_mod > MRB_FUNCALL_ARGC_MAX ? 3 : argc_mod));

  *st ++ = recv;

  if (argc_mod > MRB_FUNCALL_ARGC_MAX) {
    ci->argc = -1;
    *st = mrb_ary_new_from_values(mrb, argc_mod, argv);

    if (mid != 0) {
      mrb_ary_unshift(mrb, *st, mrb_symbol_value(mid));
    }

    st ++;
  } else {
    ci->argc = argc_mod;

    if (mid != 0) {
      *st ++ = mrb_symbol_value(mid);
    }

    for (; argc > 0; argc --) {
      *st ++ = *argv ++;
    }
  }

  *st ++ = block;

  return st - mrb->c->stack;
}
