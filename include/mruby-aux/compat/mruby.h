#ifndef MRUBY_AUX_COMPAT_MRUBY_H
#define MRUBY_AUX_COMPAT_MRUBY_H 1

#include <mruby.h>

#if MRUBY_RELEASE_NO < 10300
MRB_API struct RClass *mrb_exc_get(mrb_state *mrb, const char *name);
MRB_API mrb_value mrb_vm_run(mrb_state *mrb, struct RProc *proc, mrb_value self, unsigned int stack_keep);
#endif

#if MRUBY_RELEASE_NO < 20000
MRB_API void mrb_undef_method_id(mrb_state *mrb, struct RClass *c, mrb_sym mid);
MRB_API mrb_value mrb_to_str(mrb_state *mrb, mrb_value obj);
MRB_API void mrb_stack_extend(mrb_state *mrb, mrb_int room);
#endif

#if MRUBY_RELEASE_NO < 20001
MRB_API void mrb_notimplement(mrb_state *mrb);
#endif

#if MRUBY_RELEASE_NO < 20100
MRB_INLINE const char *
mrb_sym_name(mrb_state *mrb, mrb_sym sym)
{
  return mrb_sym2name(mrb, sym);
}

MRB_INLINE const char *
mrb_sym_name_len(mrb_state *mrb, mrb_sym sym, mrb_int *len)
{
  return mrb_sym2name_len(mrb, sym, len);
}
#endif

#if MRUBY_RELEASE_NO < 20101
MRB_API mrb_noreturn void mrb_argnum_error(mrb_state *mrb, mrb_int argc, int min, int max);
#endif

#if MRUBY_RELEASE_NO < 10300
# define MRB_FIBER_RESUMED  MRB_FIBER_RESUMING
#endif

#endif /* MRUBY_AUX_COMPAT_MRUBY_H */
