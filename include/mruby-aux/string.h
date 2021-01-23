#ifndef MRUBY_AUX_STRING_H
#define MRUBY_AUX_STRING_H 1

#include "compat/string.h"
#include <string.h>
#include <stdint.h>

#ifndef MRBX_STR_MAX
# if MRB_INT_MAX < SIZE_MAX
#  define MRBX_STR_MAX (MRB_INT_MAX - 1)
#  define mrbx_size_t intptr_t
# else
#  define MRBX_STR_MAX (SIZE_MAX - 1)
#  define mrbx_size_t mrb_int
# endif
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

#elif __STDC_VERSION__ >= 201112L

# define MRBX_STR_NEW_CSTR_FUNC(CSTR)                                   \
         (MRBX_LITERAL_P(CSTR) ?                                        \
          mrbx_str_new_lit :                                            \
          mrbx_str_new_cstr)                                            \

# define mrbx_str_ptr(MRB, V)                                           \
         _Generic(V,                                                    \
                  mrb_value:             mrbx_str_ptr,                  \
                  struct RString *:      mrbx_by_str_ptr,               \
                  char *:                MRBX_STR_NEW_CSTR_FUNC(V),     \
                  const char *:          MRBX_STR_NEW_CSTR_FUNC(V)      \
         )(MRB, V)                                                      \

#endif

#define RString(V)  mrbx_str_ptr(mrb, (V))

MRBX_INLINE struct RString *
mrbx_str_set_len(mrb_state *mrb, struct RString *dest, size_t len)
{
#if SIZE_MAX >= MRB_INT_MAX
  if (len >= (size_t)MRB_INT_MAX) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "string length too large");
  }
#endif

  RSTR_SET_LEN(dest, len);

  return dest;
}

MRBX_INLINE struct RString *
mrbx_str_set_len_value(mrb_state *mrb, mrb_value dest, size_t len)
{
  mrb_check_type(mrb, dest, MRB_TT_STRING);

  return mrbx_str_set_len(mrb, RSTRING(dest), len);
}

#ifdef __cplusplus

MRBX_INLINE struct RString *
mrbx_str_set_len(mrb_state *mrb, mrb_value dest, size_t len)
{
  return mrbx_str_set_len_value(mrb, dest, len);
}

#elif __STDC_VERSION__ >= 201112L

# define mrbx_str_set_len(MRB, DEST, LEN)                               \
         _Generic(DEST,                                                 \
                  mrb_value:         mrbx_str_set_len_value,            \
                  struct RString *:  mrbx_str_set_len                   \
         )(MRB, DEST, LEN)                                              \

#endif

MRBX_INLINE struct RString *
mrbx_str_reserve(mrb_state *mrb, struct RString *str, size_t len)
{
  if ((size_t)RSTR_CAPA(str) < len) {
#if SIZE_MAX >= MRB_INT_MAX
    if (len >= (size_t)MRB_INT_MAX) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "string capacity too large");
    }
#endif

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

#elif __STDC_VERSION__ >= 201112L

# define mrbx_str_reserve(MRB, STR, LEN)                                \
         _Generic((STR),                                                \
                 mrb_value: mrbx_str_reserve_value,                     \
                 struct RString *: mrbx_str_reserve                     \
         )(MRB, STR, LEN)                                               \

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

#ifdef __cplusplus

MRBX_INLINE struct RString *
mrbx_str_recycle(mrb_state *mrb, mrb_value str, size_t len)
{
  return mrbx_str_recycle_value(mrb, str, len);
}

#elif __STDC_VERSION__ >= 201112L

# define mrbx_str_recycle(MRB, STR, LEN)                                \
         _Generic((STR),                                                \
                  mrb_value: mrbx_str_recycle_value,                    \
                  struct RString *: mrbx_str_recycle                    \
         )(MRB, STR, LEN)                                               \

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
    return mrbx_str_reserve(mrb, mrb_str_ptr(str), len);
  }
}

#ifdef __cplusplus

MRBX_INLINE struct RString *
mrbx_str_force_recycle(mrb_state *mrb, mrb_value str, size_t len)
{
  return mrbx_str_force_recycle_value(mrb, str, len);
}

#elif __STDC_VERSION__ >= 201112L

# define mrbx_str_force_recycle(MRB, STR, LEN)                          \
         _Generic(STR,                                                  \
                  struct RString *:   mrbx_str_force_recycle,           \
                  mrb_value:          mrbx_str_force_recycle_value      \
         )(MRB, STR, LEN)                                               \

#endif

MRB_BEGIN_DECL

/*
 * 一つのメモリブロックに複数のポインタ配列と文字列から構成されたものを mruby の文字列として確保して返す。
 *
 * ポインタ配列の最後には NULL が置かれる。
 *
 * [ [str1ptr] [str2ptr] ... [strNptr] [NULL] [str1...] [NUL] [str2...] [NUL] ... [strN...] [NUL] ]
 */
MRB_API struct RString *mrbx_str_new_table(mrb_state *mrb, struct RArray *list);

MRB_API struct RString *mrbx_str_drop(mrb_state *mrb, struct RString *str, mrb_int off, mrb_int size);

MRB_API char *mrbx_cstr_hexdigest(char buf[], uint64_t n, int bytesize);
MRB_API char *mrbx_cstr_HEXDIGEST(char buf[], uint64_t n, int bytesize);
MRB_API struct RString *mrbx_str_new_as_hexdigest(mrb_state *mrb, uint64_t n, int bytesize);
MRB_API struct RString *mrbx_str_new_as_HEXDIGEST(mrb_state *mrb, uint64_t n, int bytesize);

/*
 * len が0未満の場合、str から長さを取得します。
 */
MRB_API mrb_value mrbx_str_new_static(mrb_state *mrb, const char *str, intptr_t len);

#define MRBX_RSTR_EMBED_GETMEM(STR, LENP) (*(LENP) = RSTR_EMBED_LEN(STR), RSTR_EMBED_PTR(STR))
#define MRBX_RSTR_HEAP_GETMEM(STR, LENP) (*(LENP) = (STR)->as.heap.len, (STR)->as.heap.ptr)
#define MRBX_RSTR_GETMEM(STR, LENP) ((RSTR_EMBED_P(STR)) ? MRBX_RSTR_EMBED_GETMEM(STR, LENP) : MRBX_RSTR_HEAP_GETMEM(STR, LENP))

MRB_INLINE char *
mrbx_str_getmem(mrb_value str, mrb_int *lenp)
{
  return MRBX_RSTR_GETMEM(mrb_str_ptr(str), lenp);
}

MRB_END_DECL

#include "string/growup.h"

#endif /* MRUBY_AUX_STRING_H */
