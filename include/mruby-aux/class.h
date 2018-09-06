#ifndef MRUBY_AUX_CLASS_H
#define MRUBY_AUX_CLASS_H 1

#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/class.h>
#include "common.h"

MRBX_INLINE struct RClass *
mrbx_class_ptr(mrb_state *mrb, mrb_value v)
{
    switch (mrb_type(v)) {
    default:
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "expect nil, module or class, but given %S",
                   mrb_obj_value(mrb_obj_class(mrb, v)));
    case MRB_TT_NIL:
        return NULL;
    case MRB_TT_MODULE:
    case MRB_TT_CLASS:
    case MRB_TT_SCLASS:
        return mrb_class_ptr(v);
    }
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
mrbx_dig_class(mrb_state *mrb, struct RClass *c, size_t num, const char *names[])
{
    if (!c) {
        c = mrb->object_class;
    }

    for (; num > 0; num --, names ++) {
        c = mrbx_class_ptr(mrb, mrb_const_get(mrb, mrb_obj_value(c), mrb_intern_cstr(names[0])));
    }

    return c;
}

#define MRBX_DIG_CLASS(MRB, TOP, ...)                                       \
     mrbx_dig_class(MRB, RClass(TOP),                                       \
             ELEMENTOF(((const char *[]) { __VA_ARGS__ })),                 \
             (const char *[]) { __VA_ARGS__ })                              \

#endif /* MRUBY_AUX_CLASS_H */
