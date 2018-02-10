#ifndef MRUBY_AUX_ARRAY_H__
#define MRUBY_AUX_ARRAY_H__ 1

#include <mruby/array.h>
#include "compat/array.h"

#define MRBX_TUPLE(...)                                         \
    mrb_ary_new_from_values(                                    \
            mrb,                                                \
            ELEMENTOF(((const mrb_value []) { __VA_ARGS__ })),  \
            ((const mrb_value []) { __VA_ARGS__ }))             \

#define RArray(V)   _mrbx_ary_ptr(V)(mrb, (V))

static MRBX_UNUSED struct RArray *
_mrbx_ary_ptr(mrb_state *mrb, mrb_value v)
{
    if (mrb_nil_p(v)) {
        return (struct RArray *)NULL;
    } else {
        mrb_check_type(mrb, v, MRB_TT_ARRAY);
        return mrb_ary_ptr(v);
    }
}

static MRBX_UNUSED struct RArray *
_mrbx_by_ary_ptr(mrb_state *mrb, struct RArray *p)
{
    return p;
}

#ifdef __cplusplus

template <typename T> static MRBX_UNUSED struct RArray *_mrbx_ary_ptr(mrb_state *mrb, T v) { static_assert(sizeof(T) < 0, "wrong type"); }
static MRBX_UNUSED struct RArray *_mrbx_ary_ptr(mrb_state *mrb, struct RArray *p) { return _mrbx_by_ary_ptr(mrb, p); }

#   define _mrbx_ary_ptr(V) _mrbx_ary_ptr

#else

#   define _mrbx_ary_ptr(V)                             \
        _Generic((V),                                   \
                 mrb_value:         _mrbx_ary_ptr,      \
                 struct RArray *:   _mrbx_by_ary_ptr)   \

#endif

#endif /* MRUBY_AUX_ARRAY_H__ */
