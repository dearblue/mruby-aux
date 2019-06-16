#include "../include/mruby-aux/compat/mruby.h"

#if MRUBY_RELEASE_NO < 10300
MRB_API struct RClass *
mrb_exc_get(mrb_state *mrb, const char *name)
{
  struct RClass *e = mrb_class_get(mrb, name);
  return e;
}
#endif
