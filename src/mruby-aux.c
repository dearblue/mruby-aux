#include <mruby.h>

#ifdef MRUBY_AUX_INTERNALS
# error Do not build with defined MRUBY_AUX_INTERNALS always
#endif

void
mrb_mruby_aux_gem_init(mrb_state *mrb)
{
}

void
mrb_mruby_aux_gem_final(mrb_state *mrb)
{
}
