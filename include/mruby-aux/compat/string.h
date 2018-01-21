#ifndef MRUBY_AUX_COMPAT_STRING_H__
#define MRUBY_AUX_COMPAT_STRING_H__ 1

#include <mruby/string.h>

#if MRUBY_RELEASE_NO < 10200
#   define RSTR_FROZEN_P(S) FALSE
#elif MRUBY_RELEASE_NO < 10300
#else
#   define RSTR_FROZEN_P(S) MRB_FROZEN_P(S)
#endif

#if MRUBY_RELEASE_NO < 10400
static inline mrb_value
mrb_str_new_capa(mrb_state *mrb, size_t capa)
{
    return mrb_str_buf_new(mrb, capa);
}
#endif

#endif /* MRUBY_AUX_COMPAT_STRING_H__ */
