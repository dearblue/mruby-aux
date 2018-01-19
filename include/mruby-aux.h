#ifndef MRUBY_AUX_H__
#define MRUBY_AUX_H__ 1

#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>

#define ELEMENTOF(A)    (sizeof((A)) / sizeof((A)[0]))
#define ENDOF(A)        ((A) + ELEMENTOF((A)))

#define VALUE           mrb_value
#define Qnil            mrb_nil_value()
#define Qtrue           mrb_true_value()
#define Qfalse          mrb_false_value()
#define Qundef          mrb_undef_value()
#define NIL_P(obj)      mrb_nil_p(obj)
#define RTEST(O)        mrb_test(O)
#define mrb_cObject     (mrb->object_class)

#define AUX_STR_MAX     (MRB_INT_MAX - 1)

#define MRB             mrb_state *mrb

#if MRUBY_RELEASE_NO >= 10300
#   define RSTR_FROZEN_P(S) MRB_FROZEN_P(S)
#elif MRUBY_RELEASE_NO >= 10200
#   define MRB_FROZEN_P(S)  ((S)->tt == MRB_TT_STRING ? RSTR_FROZEN_P((S)) : FALSE)
#else
#   define RSTR_FROZEN_P(S) FALSE
#   define MRB_FROZEN_P(S)  FALSE
#endif

#define MRBX_TUPLE(...)                                         \
    mrb_ary_new_from_values(                                    \
            mrb,                                                \
            ELEMENTOF(((const mrb_value []) { __VA_ARGS__ })),  \
            ((const mrb_value []) { __VA_ARGS__ }))             \

static inline mrb_value
_aux_mrb_obj_value(mrb_state *mrb, void *v)
{
    return mrb_obj_value(v);
}

static inline mrb_value
_aux_to_value(mrb_state *mrb, mrb_value v)
{
    return v;
}

static inline mrb_value
_aux_mrb_fixnum_value(mrb_state *mrb, mrb_int v)
{
    return mrb_fixnum_value(v);
}

#define mrb_value(V)                                        \
    (_Generic((V),                                          \
              mrb_value:            _aux_to_value,          \
              struct RBasic *:      _aux_mrb_obj_value,     \
              struct RObject *:     _aux_mrb_obj_value,     \
              struct RClass *:      _aux_mrb_obj_value,     \
              struct RArray *:      _aux_mrb_obj_value,     \
              struct RString *:     _aux_mrb_obj_value,     \
              struct RProc *:       _aux_mrb_obj_value,     \
              struct RRange *:      _aux_mrb_obj_value,     \
              struct RFiber *:      _aux_mrb_obj_value,     \
              struct RException *:  _aux_mrb_obj_value,     \
              mrb_int:              _aux_mrb_fixnum_value,  \
              const mrb_int:        _aux_mrb_fixnum_value,  \
              mrb_float:            mrb_float_value,        \
              const mrb_float:      mrb_float_value,        \
              char *:               mrb_str_new_cstr,       \
              const char *:         mrb_str_new_cstr)       \
        (mrb, (V)))                                         \


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


static inline struct RArray *
_aux_mrb_ary_ptr(mrb_state *mrb, mrb_value v)
{
    if (mrb_nil_p(v)) {
        return (struct RArray *)NULL;
    } else {
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

static inline struct RClass *
_aux_mrb_class_ptr(mrb_state *mrb, mrb_value v)
{
    return mrb_class_ptr(v);
}

static inline struct RClass *
_aux_to_class_ptr(mrb_state *mrb, struct RClass *p)
{
    return p;
}

#define RClass(V)                                   \
    _Generic((V),                                   \
             mrb_value:         _aux_mrb_class_ptr, \
             struct RClass *:   _aux_to_class_ptr)  \
        (mrb, (V))                                  \

static inline mrb_sym
_aux_mrb_sym(mrb_state *mrb, mrb_value sym)
{
    return mrb_symbol(sym);
}

static inline mrb_sym
_aux_symbol(mrb_state *mrb, mrb_sym sym)
{
    return sym;
}

#define SYMBOL(SYM)                             \
    _Generic((SYM),                             \
             mrb_value:     _aux_mrb_sym,       \
             mrb_sym:       _aux_symbol,        \
             const mrb_sym: _aux_symbol,        \
             char *:        mrb_intern_cstr,    \
             const char *:  mrb_intern_cstr)    \
        (mrb, (SYM))                            \

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

#define FOREACH_LIST(I, TYPE, ...)                              \
    for (const TYPE _list_[] = { __VA_ARGS__ },                 \
                    *_list_end_ = _list_ + ELEMENTOF(_list_),   \
                    I = _list_;                                 \
         (&I) < _list_end_;                                     \
         (&I) ++)                                               \

#define FOREACH_NLIST(I, TYPE, N, LIST)                         \
    for (TYPE *_list_end_ = (LIST) + (N),                       \
              I = (LIST);                                       \
         (&I) < _list_end_;                                     \
         (&I) ++)                                               \

#define FOREACH_RARRAY(I, LIST)                                     \
    for (const VALUE *_list_end_ = (&I) + RARY_LEN(RArray((LIST))), \
                     I = RARY_PTR(RArray((LIST)));                  \
         (&I) < _list_end_;                                         \
         (&I) ++)                                                   \

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

#endif /* MRUBY_AUX_H__ */
