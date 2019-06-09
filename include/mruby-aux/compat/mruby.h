#ifndef MRUBY_AUX_COMPAT_MRUBY_H
#define MRUBY_AUX_COMPAT_MRUBY_H 1

#include <mruby.h>

#if MRUBY_RELEASE_NO < 10300
MRB_API struct RClass *mrb_exc_get(mrb_state *mrb, const char *name);
#endif

#endif /* MRUBY_AUX_COMPAT_MRUBY_H */
