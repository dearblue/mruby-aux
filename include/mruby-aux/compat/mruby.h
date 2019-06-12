#ifndef MRUBY_AUX_COMPAT_MRUBY_H
#define MRUBY_AUX_COMPAT_MRUBY_H 1

#include <mruby.h>

#if MRUBY_RELEASE_NO < 10300
MRB_API struct RClass *mrb_exc_get(mrb_state *mrb, const char *name);
#endif

#if MRUBY_RELEASE_NO < 20000
MRB_API void mrb_undef_method_id(mrb_state *mrb, struct RClass *c, mrb_sym mid);
#endif

#if MRUBY_RELEASE_NO < 20001
MRB_API void mrb_notimplement(mrb_state *mrb);
#endif

#endif /* MRUBY_AUX_COMPAT_MRUBY_H */
