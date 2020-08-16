#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/proc.h>
#include <mruby/array.h>
#include <stdlib.h>

#ifndef MRB_FUNCALL_ARGC_MAX
# define MRB_FUNCALL_ARGC_MAX 16
#endif

int
mrbx_vm_set_args(mrb_state *mrb, mrb_callinfo *ci, mrb_value recv, int argc, const mrb_value argv[], mrb_value block, mrb_sym mid)
{
  int needblock = 1;
  int argc_mod = (mid != 0 ? 1 : 0) + argc;
  mrb_value *st = mrbx_vm_stack_extend(mrb, (argc_mod > MRB_FUNCALL_ARGC_MAX ? 3 : argc_mod + needblock));

  *st ++ = recv;

  mrb_value *ap;
  if (argc_mod > MRB_FUNCALL_ARGC_MAX) {
    ci->argc = -1;
    *st = mrb_ary_new_capa(mrb, argc_mod);
    ap = (mrb_value *)RARRAY_PTR((*st));
    st ++;
  } else {
    ci->argc = argc_mod;
    ap = st;
    st += argc_mod;
  }

  if (mid != 0) {
    *ap ++ = mrb_symbol_value(mid);
  }

  memmove(ap, argv, sizeof(mrb_value) * argc);

  *st ++ = block;

  return st - mrb->c->stack;
}
