#ifndef MRUBY_AUX_STRING_H
#define MRUBY_AUX_STRING_H 1

#include "compat/string.h"
#include <string.h>

#ifndef MRBX_STR_MAX
#   if MRB_INT_MAX < SIZE_MAX
#       define MRBX_STR_MAX (MRB_INT_MAX - 1)
#       define mrbx_size_t ssize_t
#   else
#       define MRBX_STR_MAX (SIZE_MAX - 1)
#       define mrbx_size_t mrb_int
#   endif
#endif

MRBX_INLINE void
mrbx_str_check_size(mrb_state *mrb, mrbx_size_t size)
{
    if (size < 0 || size > MRBX_STR_MAX) {
        mrb_raise(mrb, E_RUNTIME_ERROR, "string length too large or negative");
    }
}


MRBX_INLINE struct RString *
mrbx_str_ptr(mrb_state *mrb, mrb_value v)
{
    if (mrb_nil_p(v)) {
        return (struct RString *)NULL;
    } else {
        mrb_check_type(mrb, v, MRB_TT_STRING);
        return mrb_str_ptr(v);
    }
}

MRBX_INLINE struct RString *
mrbx_by_str_ptr(mrb_state *mrb, struct RString *p)
{
    return p;
}

MRBX_INLINE mrb_value
mrbx_value_str_new_lit(mrb_state *mrb, const char *str)
{
    return mrb_str_new_static(mrb, str, strlen(str));
}

MRBX_INLINE mrb_value
mrbx_value_str_new_cstr(mrb_state *mrb, const char *str)
{
    if (MRBX_LITERAL_P(str)) {
        return mrbx_value_str_new_lit(mrb, str);
    } else {
        return mrb_str_new_cstr(mrb, str);
    }
}

MRBX_INLINE struct RString *
mrbx_str_new_lit(mrb_state *mrb, const char *str)
{
    return mrb_str_ptr(mrbx_value_str_new_lit(mrb, str));
}

MRBX_INLINE struct RString *
mrbx_str_new_cstr(mrb_state *mrb, const char *str)
{
    return mrb_str_ptr(mrbx_value_str_new_cstr(mrb, str));
}

#ifdef __cplusplus

MRBX_INLINE struct RString * mrbx_str_ptr(mrb_state *mrb, struct RString *p) { return mrbx_by_str_ptr(mrb, p); }
MRBX_INLINE struct RString * mrbx_str_ptr(mrb_state *mrb, const char *str) { return mrbx_str_new_cstr(mrb, str); }

#else

# define MRBX_STR_NEW_CSTR_FUNC(CSTR)                                       \
        (MRBX_LITERAL_P(CSTR) ?                                             \
         mrbx_str_new_lit :                                                 \
         mrbx_str_new_cstr)                                                 \

# define mrbx_str_ptr(MRB, V)                                               \
        _Generic((V),                                                       \
                 mrb_value:             mrbx_str_ptr,                       \
                 struct RString *:      mrbx_by_str_ptr,                    \
                 char *:                MRBX_STR_NEW_CSTR_FUNC(V),          \
                 const char *:          MRBX_STR_NEW_CSTR_FUNC(V))          \
            (MRB, V)                                                        \

#endif

#define RString(V)  mrbx_str_ptr(mrb, (V))

MRBX_INLINE struct RString *
mrbx_str_set_len(mrb_state *mrb, struct RString *dest, size_t len)
{
    if (len >= MRB_INT_MAX) {
            mrb_raise(mrb, E_RUNTIME_ERROR, "string length too large");
    }

    RSTR_SET_LEN(dest, len);

    return dest;
}

MRBX_INLINE struct RString *
mrbx_str_set_len_value(mrb_state *mrb, mrb_value dest, size_t len)
{
    mrb_check_type(mrb, dest, MRB_TT_STRING);

    return mrbx_str_set_len(mrb, RSTRING(dest), len);
}

#if __cplusplus

MRBX_INLINE struct RString *
mrbx_str_set_len(mrb_state *mrb, mrb_value dest, size_t len)
{
    return mrbx_str_set_len_value(mrb, dest, len);
}

#else

#   define mrbx_str_set_len(MRB, DEST, LEN)                                 \
        _Generic((DEST),                                                    \
                 mrb_value:         mrbx_str_set_len_value,                 \
                 struct RString *:  mrbx_str_set_len)                       \
            ((MRB), (DEST), (LEN))                                          \

#endif

MRBX_INLINE struct RString *
mrbx_str_reserve(mrb_state *mrb, struct RString *str, size_t len)
{
    if (RSTR_CAPA(str) < len) {
        if (len >= MRB_INT_MAX) {
            mrb_raise(mrb, E_RUNTIME_ERROR, "string capacity too large");
        }

        mrb_int l = RSTR_LEN(str);
        mrb_str_resize(mrb, mrb_obj_value(str), len);
        RSTR_SET_LEN(str, l);
    } else {
        mrb_str_modify(mrb, str);
    }

    return str;
}

MRBX_INLINE struct RString *
mrbx_str_reserve_value(mrb_state *mrb, mrb_value str, size_t len)
{
    mrb_check_type(mrb, str, MRB_TT_STRING);

    return mrbx_str_reserve(mrb, RSTRING(str), len);
}

#ifdef __cplusplus

MRBX_INLINE struct RString *
mrbx_str_reserve(mrb_state *mrb, mrb_value str, size_t len)
{
    return mrbx_str_reserve_value(mrb, str, len);
}

#else

#   define mrbx_str_reserve(MRB, STR, LEN)                                  \
        _Generic((STR),                                                     \
                mrb_value: mrbx_str_reserve_value,                          \
                struct RString *: mrbx_str_reserve)                         \
            ((MRB), (STR), (LEN))                                           \

#endif

MRBX_INLINE struct RString *
mrbx_str_recycle(mrb_state *mrb, struct RString *str, size_t len)
{
    if (str && !MRB_FROZEN_P(str)) {
        return mrbx_str_reserve(mrb, str, len);
    } else {
        return RSTRING(mrb_str_buf_new(mrb, len));
    }
}

MRBX_INLINE struct RString *
mrbx_str_recycle_value(mrb_state *mrb, mrb_value str, size_t len)
{
    if (mrb_nil_p(str)) {
        return mrbx_str_recycle(mrb, NULL, len);
    } else {
        mrb_check_type(mrb, str, MRB_TT_STRING);
        return mrbx_str_recycle(mrb, RSTRING(str), len);
    }
}

#if __cplusplus

MRBX_INLINE struct RString *
mrbx_str_recycle(mrb_state *mrb, mrb_value str, size_t len)
{
    return mrbx_str_recycle_value(mrb, str, len);
}

#else

#   define mrbx_str_recycle(MRB, STR, LEN)                                  \
        _Generic((STR),                                                     \
                mrb_value: mrbx_str_recycle_value,                          \
                struct RString *: mrbx_str_recycle)                         \
            ((MRB), (STR), (LEN))                                           \

#endif

MRBX_INLINE struct RString *
mrbx_str_force_recycle(mrb_state *mrb, struct RString *str, size_t len)
{
    if (!str) {
        return RSTRING(mrb_str_buf_new(mrb, len));
    } else {
        return mrbx_str_reserve(mrb, str, len);
    }
}

MRBX_INLINE struct RString *
mrbx_str_force_recycle_value(mrb_state *mrb, mrb_value str, size_t len)
{
    if (mrb_nil_p(str)) {
        return RSTRING(mrb_str_buf_new(mrb, len));
    } else {
        mrb_check_type(mrb, str, MRB_TT_STRING);
        return mrbx_str_reserve(mrb, RString(str), len);
    }
}

#if __cplusplus

MRBX_INLINE struct RString *
mrbx_str_force_recycle(mrb_state *mrb, mrb_value str, size_t len)
{
    return mrbx_str_force_recycle_value(mrb, str, len);
}

#else

#   define mrbx_str_force_recycle(MRB, STR, LEN)                            \
        _Generic((STR),                                                     \
                struct RString *:   mrbx_str_force_recycle,                 \
                mrb_value:          mrbx_str_force_recycle_value)           \
            ((MRB), (STR), (LEN))                                           \

#endif

#endif /* MRUBY_AUX_STRING_H */
