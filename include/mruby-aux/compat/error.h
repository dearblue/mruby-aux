#ifndef MRUBY_AUX_COMPAT_ERROR_H
#define MRUBY_AUX_COMPAT_ERROR_H 1

#include <mruby.h>
#include <mruby/error.h>
#include <mruby/version.h>

#if MRUBY_RELEASE_NO <= 30000

MRB_BEGIN_DECL

typedef mrb_value mrb_protect_error_func(mrb_state *mrb, void *userdata);
MRB_API mrb_value mrb_protect_error(mrb_state *mrb, mrb_protect_error_func *body, void *userdata, mrb_bool *error);

MRB_END_DECL

#endif // MRUBY_RELEASE_NO <= 30000

#endif // MRUBY_AUX_COMPAT_ERROR_H
