#include "../include/mruby-aux/compat/mruby.h"

#if MRUBY_RELEASE_NO < 20000
MRB_API void
mrb_undef_method_id(mrb_state *mrb, struct RClass *c, mrb_sym mid)
{
  mrb_undef_method(mrb, c, mrb_sym2name(mrb, mid));
}
#endif
