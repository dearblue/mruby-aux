#ifndef MRUBY_AUX_COMPAT_STRING_H
#define MRUBY_AUX_COMPAT_STRING_H 1

#include "../common.h"
#include <mruby.h>
#include <mruby/string.h>

#if MRUBY_RELEASE_NO < 10400
MRBX_INLINE mrb_value
mrb_str_new_capa(mrb_state *mrb, size_t capa)
{
    return mrb_str_buf_new(mrb, capa);
}
#endif

#ifndef RSTR_END
# define RSTR_END(S) (RSTR_PTR(S) + RSTR_LEN(S))
#endif

#endif /* MRUBY_AUX_COMPAT_STRING_H */
