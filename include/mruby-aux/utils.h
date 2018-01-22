#ifndef MRUBY_AUX_UTILS_H__
#define MRUBY_AUX_UTILS_H__ 1

#include <mruby.h>
#include <mruby/value.h>
#include <mruby/data.h>

#ifndef ELEMENTOF
#   define ELEMENTOF(A) (sizeof((A)) / sizeof((A)[0]))
#endif

#ifndef ENDOF
#   define ENDOF(A)     ((A) + ELEMENTOF((A)))
#endif

#define IMPLEMENT_ME                                \
        mrb_raisef(mrb, E_NOTIMP_ERROR,             \
                   "IMPLEMENT ME! (in %S:%S:%S)",   \
                   mrb_str_new_cstr(mrb, __FILE__), \
                   mrb_fixnum_value(__LINE__),      \
                   mrb_str_new_cstr(mrb, __func__)) \

#define FUNCALL(MRB, RECV, MID, ...)                            \
    mrb_funcall_argv(                                           \
            (MRB), (RECV), SYMBOL((MID)),                       \
            ELEMENTOF(((const mrb_value []) { __VA_ARGS__ })),  \
            ((const mrb_value []) { __VA_ARGS__ }))             \

#define FUNCALL_WITH_BLOCK(MRB, RECV, MID, BLOCK, ...)          \
    mrb_funcall_with_block(                                     \
            (MRB), (RECV), SYMBOL((MID)),                       \
            ELEMENTOF(((const mrb_value []) { __VA_ARGS__ })),  \
            ((const mrb_value []) { __VA_ARGS__ }),             \
            (BLOCK))                                            \

#define FOREACH_LIST(TYPE, I, ...)                              \
    for (TYPE _list_[] = { __VA_ARGS__ },                       \
              *_list_end_ = _list_ + ELEMENTOF(_list_),         \
              I = _list_;                                       \
         &I < _list_end_;                                       \
         I ++)                                                  \

#define FOREACH_ALIST(TYPE, I, LIST)                            \
    for (TYPE *_list_end_ = (LIST) + ELEMENTOF((LIST)),         \
              I = (LIST);                                       \
         &I < _list_end_;                                       \
         I ++)                                                  \

#define FOREACH_NLIST(TYPE, N, I, LIST)                         \
    for (TYPE *_list_end_ = (LIST) + (N),                       \
              I = (LIST);                                       \
         &I < _list_end_;                                       \
         I ++)                                                  \

#define FOREACH_RARRAY(I, LIST)                                     \
    for (const VALUE I = ARY_PTR(RArray((LIST))),                   \
                     *_list_end_ = (&I) + ARY_LEN(RArray((LIST)));  \
         &I < _list_end_;                                           \
         I ++)                                                      \

static inline void *
mrbx_getrefp(MRB, VALUE obj, const mrb_data_type *type)
{
    void *p;

    Data_Get_Struct(mrb, obj, type, p);

    return p;
}

static inline void *
mrbx_getref(MRB, VALUE obj, const mrb_data_type *type)
{
    void *p = mrbx_getrefp(mrb, obj, type);

    if (!p) {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "invalid initialized pointer - %S",
                   obj);
    }

    return p;
}

static inline mrb_value
mrbx_funcall_passthrough(MRB, mrb_value recv, mrb_sym mid)
{
    mrb_int argc;
    mrb_value *argv, block;
    mrb_get_args(mrb, "*&", &argv, &argc, &block);
    return mrb_funcall_with_block(mrb, recv, mid, argc, argv, block);
}

static inline mrb_value
mrbx_instance_eval(mrb_state *mrb, mrb_value o, mrb_value b)
{
    return mrb_yield_with_class(mrb, b, 1, &b, o, NULL);
}

static inline mrb_value
mrbx_instance_exec(mrb_state *mrb, mrb_value o, mrb_value b, mrb_int argc, const mrb_value argv[])
{
    return mrb_yield_with_class(mrb, b, argc, argv, o, NULL);
}

#endif /* MRUBY_AUX_UTILS_H__ */
