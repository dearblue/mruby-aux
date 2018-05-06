#ifndef MRUBY_AUX_ARRAY_H
#define MRUBY_AUX_ARRAY_H 1

#include <mruby/array.h>
#include "compat/array.h"

#define MRBX_TUPLE(...)                                         \
    mrb_ary_new_from_values(                                    \
            mrb,                                                \
            ELEMENTOF(((const mrb_value []) { __VA_ARGS__ })),  \
            ((const mrb_value []) { __VA_ARGS__ }))             \

#define RArray(V)   mrbx_ary_ptr(mrb, (V))

MRBX_UNUSED static struct RArray *
mrbx_ary_ptr(mrb_state *mrb, mrb_value v)
{
    if (mrb_nil_p(v)) {
        return (struct RArray *)NULL;
    } else {
        mrb_check_type(mrb, v, MRB_TT_ARRAY);
        return mrb_ary_ptr(v);
    }
}

MRBX_UNUSED static struct RArray *
mrbx_by_ary_ptr(mrb_state *mrb, struct RArray *p)
{
    return p;
}

#ifdef __cplusplus

MRBX_UNUSED static struct RArray *mrbx_ary_ptr(mrb_state *mrb, struct RArray *p) { return mrbx_by_ary_ptr(mrb, p); }

#else

#   define mrbx_ary_ptr(MRB, V)                     \
        _Generic((V),                               \
                 mrb_value:         mrbx_ary_ptr,   \
                 struct RArray *:   mrbx_by_ary_ptr \
                 )(MRB, V)                          \

#endif

#endif /* MRUBY_AUX_ARRAY_H */
