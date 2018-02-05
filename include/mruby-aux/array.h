#ifndef MRUBY_AUX_ARRAY_H__
#define MRUBY_AUX_ARRAY_H__ 1

#include <mruby/array.h>
#include "compat/array.h"

#define MRBX_TUPLE(...)                                         \
    mrb_ary_new_from_values(                                    \
            mrb,                                                \
            ELEMENTOF(((const mrb_value []) { __VA_ARGS__ })),  \
            ((const mrb_value []) { __VA_ARGS__ }))             \

static inline struct RArray *
_aux_mrb_ary_ptr(mrb_state *mrb, mrb_value v)
{
    if (mrb_nil_p(v)) {
        return (struct RArray *)NULL;
    } else {
        mrb_check_type(mrb, v, MRB_TT_ARRAY);
        return mrb_ary_ptr(v);
    }
}

static inline struct RArray *
_aux_to_ary_ptr(mrb_state *mrb, struct RArray *p)
{
    return p;
}

#define RArray(V)                                   \
    _Generic((V),                                   \
             mrb_value:         _aux_mrb_ary_ptr,   \
             struct RArray *:   _aux_to_ary_ptr)    \
        (mrb, (V))                                  \

#endif /* MRUBY_AUX_ARRAY_H__ */
