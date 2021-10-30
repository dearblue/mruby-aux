#include <mruby-aux/vmext.h>
#include <mruby-aux/compat/proc.h>
#include <mruby/array.h>
#include "vm_internals.h"
#include <stdlib.h>

int
mrbx_vm_set_args(mrb_state *mrb, mrb_callinfo *ci, mrb_value recv, int argc, const mrb_value argv[], mrb_value block, mrb_sym mid)
{
  int needblock = 1;
  int argc_mod = (mid != 0 ? 1 : 0) + argc;
  mrb_value *st = mrbx_vm_stack_extend(mrb, (argc_mod > ARGS_FORCE_PACK_THRESHOLD ? 3 : argc_mod + needblock));

  *st++ = recv;

  mrb_value *ap;
  if (argc_mod > ARGS_FORCE_PACK_THRESHOLD) {
    CI_ARGC_SET_PACKED(ci);
    *st = mrb_ary_new_capa(mrb, argc_mod);
    ap = (mrb_value *)RARRAY_PTR((*st));
    st++;
  } else {
    CI_ARGC_SET(ci, argc_mod);
    ap = st;
    st += argc_mod;
  }

  if (mid != 0) {
    *ap++ = mrb_symbol_value(mid);
  }

  memmove(ap, argv, sizeof(mrb_value) * argc);
  if (CI_ARGS_PACKED_P(ci)) {
    mrb_value splat_args = mrbx_vm_top_stacks(mrb->c)[1];
    mrb_write_barrier(mrb, (struct RBasic *)mrb_obj_ptr(splat_args));
  }

  *st++ = block;

  return st - mrbx_vm_top_stacks(mrb->c);
}
