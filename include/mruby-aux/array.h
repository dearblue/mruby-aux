#ifndef MRUBY_AUX_ARRAY_H
#define MRUBY_AUX_ARRAY_H 1

#include "compat/array.h"

#define MRBX_TUPLE(...)                                                 \
        mrb_ary_new_from_values(mrb,                                    \
                                MRBX_LIST(const mrb_value, __VA_ARGS__)) \

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

MRB_INLINE struct RArray *
mrbx_expect_ary_ptr(mrb_state *mrb, struct RArray *p)
{
  if (!p) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "unexpected NULL pointer for Array");
  }

  return p;
}

MRB_INLINE struct RArray *
mrbx_expect_ary_ptr_value(mrb_state *mrb, mrb_value v)
{
  mrb_check_type(mrb, v, MRB_TT_ARRAY);
  return mrbx_expect_ary_ptr(mrb, mrb_ary_ptr(v));
}

#ifdef __cplusplus

MRBX_INLINE struct RArray *mrbx_ary_ptr(mrb_state *mrb, struct RArray *p) { return mrbx_by_ary_ptr(mrb, p); }
MRB_INLINE struct RArray *mrbx_expect_ary_ptr(mrb_state *mrb, mrb_value v) { return mrbx_expect_ary_ptr_value(mrb, v); }

#elif __STDC_VERSION__ >= 201112L

# define mrbx_ary_ptr(MRB, V)                                           \
         _Generic(V,                                                    \
                  mrb_value:         mrbx_ary_ptr,                      \
                  struct RArray *:   mrbx_by_ary_ptr                    \
         )(MRB, V)                                                      \

# define mrbx_expect_ary_ptr(MRB, V)                                    \
         _Generic((V),                                                  \
                  mrb_value:         mrbx_expect_ary_ptr_value,         \
                  struct RArray *:   mrbx_expect_ary_ptr                \
         )(MRB, V)                                                      \

#endif

#define MRBX_JOIN(SEP, ...)                                             \
        mrb_ary_join(mrb, MRBX_TUPLE(__VA_ARGS__), mrb_value(RString(SEP)))

MRB_API void mrbx_ary_set_len(mrb_state *mrb, struct RArray *ary, mrb_int new_len);
MRB_API mrb_value mrbx_ary_delete(mrb_state *mrb, struct RArray *ary, mrb_value obj);
MRB_API mrb_value mrbx_ary_delete_at(mrb_state *mrb, struct RArray *ary, mrb_int idx);
MRB_API mrb_value mrbx_ary_delete_once(mrb_state *mrb, struct RArray *ary, mrb_value obj);

#if MRUBY_RELEASE_NO < 10400
# define MRBX_RARY_HEAP_LEN(A)  ((A)->len)
# define MRBX_RARY_HEAP_PTR(A)  ((A)->ptr)
#else
# define MRBX_RARY_HEAP_LEN(A)  ((A)->as.heap.len)
# define MRBX_RARY_HEAP_PTR(A)  ((A)->as.heap.ptr)
#endif

#define MRBX_RARY_EMBED_GETMEM(ARY, LENP) (*(LENP) = ARY_EMBED_LEN(ARY), (mrb_value *)ARY_EMBED_PTR(ARY))
#define MRBX_RARY_HEAP_GETMEM(ARY, LENP) (*(LENP) = MRBX_RARY_HEAP_LEN(ARY), MRBX_RARY_HEAP_PTR(ARY))
#define MRBX_RARY_GETMEM(ARY, LENP) (ARY_EMBED_P(ARY)?  MRBX_RARY_EMBED_GETMEM(ARY, LENP) : MRBX_RARY_HEAP_GETMEM(ARY, LENP))

MRB_INLINE const mrb_value *
mrbx_ary_getmem(mrb_value ary, size_t *len)
{
  return MRBX_RARY_GETMEM(mrb_ary_ptr(ary), len);
}

MRB_INLINE const mrb_value *mrbx_expect_ary_ptr_getmem_termed(mrb_state *mrb, struct RArray *a, const mrb_value **end)
{
  a = mrbx_expect_ary_ptr(mrb, a);
  size_t len;
  const mrb_value *p = MRBX_RARY_GETMEM(a, &len);
  *end = p + len;
  return p;
}

#endif /* MRUBY_AUX_ARRAY_H */
