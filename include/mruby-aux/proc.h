#ifndef MRUBY_AUX_PROC_H
#define MRUBY_AUX_PROC_H 1

#include "common.h"
#include "compat/proc.h"

MRB_INLINE mrb_method_t
mrbx_method_wrap_proc(struct RProc *proc, mrb_aspec aspec)
{
  mrb_method_t m;
  MRB_METHOD_FROM_PROC(m, proc);
  if ((aspec & ~MRB_ARGS_BLOCK()) == 0) {
    MRB_METHOD_NOARG_SET(m);
  }
  return m;
}

#endif /* MRUBY_AUX_PROC_H */
