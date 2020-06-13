#include <mruby/version.h>

#if MRUBY_RELEASE_NO < 20101

#include <mruby-aux/compat/mruby.h>

MRB_API mrb_noreturn void
mrb_argnum_error(mrb_state *mrb, mrb_int argc, int min, int max)
{
  mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of argument");
}

#endif
