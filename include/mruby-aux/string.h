#ifndef MRUBY_AUX_STRING_H__
#define MRUBY_AUX_STRING_H__ 1

#include "compat/string.h"

#ifndef MRBX_STR_MAX
#   if MRB_INT_MAX < SIZE_MAX
#       define MRBX_STR_MAX (MRB_INT_MAX - 1)
#   else
#       define MRBX_STR_MAX (SIZE_MAX - 1)
#   endif
#endif

#ifdef __cplusplus

static inline struct RString *
_mrbx_str_ptr(mrb_state *mrb, mrb_value v)
{
    if (mrb_nil_p(v)) {
        return (struct RString *)NULL;
    } else {
        mrb_check_type(mrb, v, MRB_TT_STRING);
        return mrb_str_ptr(v);
    }
}

static inline struct RString *
_mrbx_str_ptr(mrb_state *mrb, struct RString *p)
{
    return p;
}

static inline struct RString *
_mrbx_str_ptr(mrb_state *mrb, const char *str)
{
    return mrb_str_ptr(mrb_str_new_cstr(mrb, str));
}

#   define _mrbx_str_ptr(V) _mrbx_str_ptr

#else

static inline struct RString *
_mrbx_str_ptr(mrb_state *mrb, mrb_value v)
{
    if (mrb_nil_p(v)) {
        return (struct RString *)NULL;
    } else {
        mrb_check_type(mrb, v, MRB_TT_STRING);
        return mrb_str_ptr(v);
    }
}

static inline struct RString *
_mrbx_by_str_ptr(mrb_state *mrb, struct RString *p)
{
    return p;
}

static inline struct RString *
_mrbx_str_new_cstr(mrb_state *mrb, const char *str)
{
    return mrb_str_ptr(mrb_str_new_cstr(mrb, str));
}

#define _mrbx_str_ptr(V)                                \
    _Generic((V),                                       \
             mrb_value:             _mrbx_str_ptr,      \
             struct RString *:      _mrbx_by_str_ptr,   \
             char *:                _mrbx_str_new_cstr, \
             const char *:          _mrbx_str_new_cstr) \

#endif

#define RString(V)  _mrbx_str_ptr(V)(mrb, (V))

static inline struct RString *
mrbx_str_set_len(mrb_state *mrb, struct RString *dest, size_t len)
{
    if (len >= MRB_INT_MAX) {
            mrb_raise(mrb, E_RUNTIME_ERROR, "string length too large");
    }

    RSTR_SET_LEN(dest, len);

    return dest;
}

static inline struct RString *
mrbx_str_set_len_value(mrb_state *mrb, mrb_value dest, size_t len)
{
    mrb_check_type(mrb, dest, MRB_TT_STRING);

    return mrbx_str_set_len(mrb, RSTRING(dest), len);
}

#if __cplusplus

static inline struct RString *
mrbx_str_set_len(mrb_state *mrb, mrb_value dest, size_t len)
{
    return mrbx_str_set_len_value(mrb, dest, len);
}

#else

#   define mrbx_str_set_len(MRB, DEST, LEN)                 \
        _Generic((DEST),                                    \
                 mrb_value:         mrbx_str_set_len_value, \
                 struct RString *:  mrbx_str_set_len)       \
            ((MRB), (DEST), (LEN))                          \

#endif

static inline struct RString *
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

static inline struct RString *
mrbx_str_reserve_value(mrb_state *mrb, mrb_value str, size_t len)
{
    mrb_check_type(mrb, str, MRB_TT_STRING);

    return mrbx_str_reserve(mrb, RSTRING(str), len);
}

#ifdef __cplusplus

static inline struct RString *
mrbx_str_reserve(mrb_state *mrb, mrb_value str, size_t len)
{
    return mrbx_str_reserve_value(mrb, str, len);
}

#else

#   define mrbx_str_reserve(MRB, STR, LEN)          \
        _Generic((STR),                             \
                mrb_value: mrbx_str_reserve_value,  \
                struct RString *: mrbx_str_reserve) \
            ((MRB), (STR), (LEN))                   \

#endif

static inline struct RString *
mrbx_str_recycle(mrb_state *mrb, struct RString *str, size_t len)
{
    if (str && !MRB_FROZEN_P(str)) {
        return mrbx_str_reserve(mrb, str, len);
    } else {
        return RSTRING(mrb_str_buf_new(mrb, len));
    }
}

static inline struct RString *
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

static inline struct RString *
mrbx_str_recycle(mrb_state *mrb, mrb_value str, size_t len)
{
    return mrbx_str_recycle_value(mrb, str, len);
}

#else

#   define mrbx_str_recycle(MRB, STR, LEN)          \
        _Generic((STR),                             \
                mrb_value: mrbx_str_recycle_value,  \
                struct RString *: mrbx_str_recycle) \
            ((MRB), (STR), (LEN))                   \

#endif

static inline struct RString *
mrbx_str_force_recycle(mrb_state *mrb, struct RString *str, size_t len)
{
    if (!str) {
        return RSTRING(mrb_str_buf_new(mrb, len));
    } else {
        return mrbx_str_reserve(mrb, str, len);
    }
}

static inline struct RString *
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

static inline struct RString *
mrbx_str_force_recycle(mrb_state *mrb, mrb_value str, size_t len)
{
    return mrbx_str_force_recycle_value(mrb, str, len);
}

#else

#   define mrbx_str_force_recycle(MRB, STR, LEN)                    \
        _Generic((STR),                                             \
                struct RString *:   mrbx_str_force_recycle,         \
                mrb_value:          mrbx_str_force_recycle_value)   \
            ((MRB), (STR), (LEN))                                   \

#endif

#endif /* MRUBY_AUX_STRING_H__ */
