#ifndef MRUBY_AUX_STRING_H__
#define MRUBY_AUX_STRING_H__ 1

#include "compat/string.h"

#define MRBX_STR_MAX    (MRB_INT_MAX - 1)

static inline struct RString *
_aux_mrb_str_ptr(mrb_state *mrb, mrb_value v)
{
    if (mrb_nil_p(v)) {
        return (struct RString *)NULL;
    } else {
        return mrb_str_ptr(v);
    }
}

static inline struct RString *
_aux_to_str_ptr(mrb_state *mrb, struct RString *p)
{
    return p;
}

static inline struct RString *
_aux_mrb_str_new_cstr(mrb_state *mrb, const char *str)
{
    return mrb_str_ptr(mrb_str_new_cstr(mrb, str));
}

#define RString(V)                                      \
    _Generic((V),                                       \
             mrb_value:             _aux_mrb_str_ptr,   \
             struct RString *:      _aux_to_str_ptr,    \
             char *:                _aux_mrb_str_new_cstr, \
             const char *:          _aux_mrb_str_new_cstr) \
        (mrb, (V))                                      \


static inline struct RString *
mrbx_str_reserve(mrb_state *mrb, struct RString *str, mrb_int len)
{
    if (RSTR_CAPA(str) < len) {
        mrb_int l = RSTR_LEN(str);
        mrb_str_resize(mrb, VALUE(str), len);
        RSTR_SET_LEN(str, l);
    } else {
        mrb_str_modify(mrb, str);
    }

    return str;
}

static inline struct RString *
mrbx_str_recycle(mrb_state *mrb, struct RString *str, mrb_int len)
{
    if (str && !MRB_FROZEN_P(str)) {
        return mrbx_str_reserve(mrb, str, len);
    }

    return RSTRING(mrb_str_buf_new(mrb, len));
}

#endif /* MRUBY_AUX_STRING_H__ */
