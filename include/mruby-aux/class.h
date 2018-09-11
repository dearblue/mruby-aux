#ifndef MRUBY_AUX_CLASS_H
#define MRUBY_AUX_CLASS_H 1

#include "common.h"
#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/class.h>

MRBX_INLINE int
mrbx_namespace_p(mrb_value v)
{
    switch (mrb_type(v)) {
    default:
        return FALSE;
    case MRB_TT_MODULE:
    case MRB_TT_CLASS:
    case MRB_TT_SCLASS:
        return TRUE;
    }
}

MRBX_INLINE struct RClass *
mrbx_class_ptr(mrb_state *mrb, mrb_value v)
{
    if (mrbx_namespace_p(v)) {
        return mrb_class_ptr(v);
    } else if (!mrb_nil_p(v)) {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "expect nil, module or class, but given %S",
                   mrb_obj_value(mrb_obj_class(mrb, v)));
    }

    return NULL;
}

MRBX_INLINE struct RClass *
mrbx_by_class_ptr(mrb_state *mrb, struct RClass *p)
{
    return p;
}

#ifdef __cplusplus

MRBX_INLINE struct RClass *
mrbx_class_ptr(mrb_state *mrb, struct RClass *p)
{
    return mrbx_by_class_ptr(mrb, p);
}

#else

#define mrbx_class_ptr(MRB, V)                                              \
    _Generic((V),                                                           \
             mrb_value:         mrbx_class_ptr,                             \
             struct RClass *:   mrbx_by_class_ptr                           \
            )(MRB, V)                                                       \

#endif

#define RClass(V) mrbx_class_ptr(mrb, (V))

MRBX_INLINE struct RClass *
mrbx_dig_class(mrb_state *mrb, struct RClass *c, size_t num, const mrb_sym names[])
{
    if (!c) {
        c = mrb->object_class;
    }

    for (; num > 0; num --, names ++) {
        c = mrbx_class_ptr(mrb, mrb_const_get(mrb, mrb_obj_value(c), *names));
    }

    return c;
}

#define MRBX_DIG_CLASS(MRB, TOP, ...)                                       \
        mrbx_dig_class(MRB, RClass(TOP), MRBX_LIST(const mrb_sym, __VA_ARGS__)) \


MRBX_INLINE mrb_value
mrbx_dig_const(mrb_state *mrb, struct RClass *outer, int numname, const mrb_sym name[])
{
    if (!outer) { outer = mrb->object_class; }

    mrb_value v = mrb_obj_value(outer);

    for (; numname > 0; numname --, name ++) {
        v = mrb_const_get(mrb, v, *name);
    }

    return v;
}

#define MRBX_DIG_CONST(MRB, TOP, ...)                                       \
        mrbx_dig_const(MRB, TOP, MRBX_LIST(const mrb_sym, __VA_ARGS__))     \

#endif /* MRUBY_AUX_CLASS_H */
