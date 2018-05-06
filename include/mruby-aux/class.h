#ifndef MRUBY_AUX_CLASS_H
#define MRUBY_AUX_CLASS_H 1

#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/class.h>

static inline struct RClass *
mrbx_class_ptr(mrb_state *mrb, mrb_value v)
{
    if (mrb_nil_p(v)) {
        return NULL;
    } else {
        mrb_check_type(mrb, v, MRB_TT_CLASS);
        return mrb_class_ptr(v);
    }
}

static inline struct RClass *
mrbx_by_class_ptr(mrb_state *mrb, struct RClass *p)
{
    return p;
}

#ifdef __cplusplus

static inline struct RClass *
mrbx_class_ptr(mrb_state *mrb, struct RClass *p)
{
    return mrbx_by_class_ptr(mrb, p);
}

#else

#define mrbx_class_ptr(MRB, V)                      \
    _Generic((V),                                   \
             mrb_value:         mrbx_class_ptr,     \
             struct RClass *:   mrbx_by_class_ptr   \
            )(MRB, V)                               \

#endif

#define RClass(V) mrbx_class_ptr(mrb, (V))

static inline struct RClass *
aux_dig_class(MRB, struct RClass *c, size_t num, const char *names[])
{
    if (!c) {
        c = mrb->object_class;
    }

    for (; num > 0; num --, names ++) {
        c = mrb_class_get_under(mrb, c, *names);
    }

    return c;
}

#define AUX_DIG_CLASS(MRB, TOP, ...)                        \
     aux_dig_class(MRB, RClass(TOP),                        \
             ELEMENTOF(((const char *[]) { __VA_ARGS__ })), \
             (const char *[]) { __VA_ARGS__ })              \

#endif /* MRUBY_AUX_CLASS_H */
