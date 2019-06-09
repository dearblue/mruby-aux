#ifndef MRUBY_AUX_ARRAY_H
#define MRUBY_AUX_ARRAY_H 1

#include "compat/array.h"

#define MRBX_TUPLE(...)                                                     \
    mrb_ary_new_from_values(mrb, MRBX_LIST(const mrb_value, __VA_ARGS__))   \

#define RArray(V)   mrbx_ary_ptr(mrb, (V))

MRBX_INLINE struct RArray *
mrbx_ary_ptr(mrb_state *mrb, mrb_value v)
{
    if (mrb_nil_p(v)) {
        return (struct RArray *)NULL;
    } else {
        mrb_check_type(mrb, v, MRB_TT_ARRAY);
        return mrb_ary_ptr(v);
    }
}

MRBX_INLINE struct RArray *
mrbx_by_ary_ptr(mrb_state *mrb, struct RArray *p)
{
    return p;
}

#ifdef __cplusplus

MRBX_INLINE struct RArray *mrbx_ary_ptr(mrb_state *mrb, struct RArray *p) { return mrbx_by_ary_ptr(mrb, p); }

#else

#   define mrbx_ary_ptr(MRB, V)                                             \
        _Generic((V),                                                       \
                 mrb_value:         mrbx_ary_ptr,                           \
                 struct RArray *:   mrbx_by_ary_ptr                         \
                 )(MRB, V)                                                  \

#endif

#define MRBX_JOIN(SEP, ...)                                                 \
        mrb_ary_join(mrb, MRBX_TUPLE(__VA_ARGS__), mrb_value(RString(SEP)))

MRB_API void mrbx_ary_set_len(mrb_state *mrb, struct RArray *ary, mrb_int new_len);
MRB_API mrb_value mrbx_ary_delete(mrb_state *mrb, struct RArray *ary, mrb_value obj);
MRB_API mrb_value mrbx_ary_delete_at(mrb_state *mrb, struct RArray *ary, mrb_int idx);
MRB_API mrb_value mrbx_ary_delete_once(mrb_state *mrb, struct RArray *ary, mrb_value obj);

#endif /* MRUBY_AUX_ARRAY_H */
