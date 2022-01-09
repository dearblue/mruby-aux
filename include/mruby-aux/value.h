#ifndef MRUBY_AUX_VALUE_H
#define MRUBY_AUX_VALUE_H 1

#include "common.h"
#include "compat/value.h"
#include <mruby-aux/version.h>

/* The implant immediate values */

#if defined(MRB_NAN_BOXING)

# if MRBX_MRUBY_RELEASE_NO > 30000

#  define MRBX_BOXNAN_IMPLANT_CODE(NANTT, N)                            \
          (((uint64_t)(NANTT) << 48) |                                  \
           ((uint64_t)(N) & UINT64_C(0x0000ffffffffffff)))              \

#  define MRBX_BOXNAN_IMPLANT_MISC_CODE(TT, N)                          \
          MRBX_BOXNAN_IMPLANT_CODE(MRB_NANBOX_TT_MISC,                  \
                                   ((uint64_t)(TT) << 32) | ((uint32_t)(N) & UINT32_C(0xffffffff))) \

#  define MRBX_BOXNAN_IMPLANT_TYPE(TT, N)                               \
          ((TT) == MRB_TT_UNDEF ? MRBX_BOXNAN_IMPLANT_MISC_CODE(MRB_TT_UNDEF, 4) : \
           (TT) == MRB_TT_TRUE ? MRBX_BOXNAN_IMPLANT_MISC_CODE(MRB_TT_TRUE, 1) : \
           (TT) == MRB_TT_FALSE && (N) == 0 ? MRBX_BOXNAN_IMPLANT_CODE(MRB_NANBOX_TT_OBJECT, 0) : \
           (TT) == MRB_TT_FALSE ? MRBX_BOXNAN_IMPLANT_MISC_CODE(MRB_TT_FALSE, 1) : \
           (TT) == MRB_TT_FIXNUM ? MRBX_BOXNAN_IMPLANT_CODE(MRB_NANBOX_TT_INTEGER, N) : \
           (TT) == MRB_TT_SYMBOL ? MRBX_BOXNAN_IMPLANT_MISC_CODE(MRB_TT_SYMBOL, N) : \
           MRBX_BOXNAN_IMPLANT_CODE(MRB_NANBOX_TT_OBJECT, 0))           \

# else

#  define MRBX_BOXNAN_IMPLANT_TYPE(TT, HI)                              \
          (UINT64_C(0xfff0000000000000) |                               \
           ((((TT) + UINT64_C(1)) & 0x3f) << 46) |                      \
           ((uint64_t)(HI) & UINT64_C(0x00003fffffffffff)))             \

# endif

# ifdef __cplusplus

struct mrbx_implant_value
{
  union {
    double f;
    uint64_t w;
    mrb_value value;
  };

  mrbx_implant_value(enum mrb_vtype tt, int64_t v) :
    w(MRBX_BOXNAN_IMPLANT_TYPE(tt, v))
  {
  }

  operator mrb_value(void) const
  {
    return value;
  }
};

#  define MRBX_IMPLANT_VALUE(TT, V)                                     \
          ((mrb_value)mrbx_implant_value(TT, (int64_t)(V)))             \

# else

#  if MRBX_MRUBY_RELEASE_NO > 30000

#   define MRBX_IMPLANT_VALUE(TT, V) { MRBX_BOXNAN_IMPLANT_TYPE(TT, V) }

#  elif MRUBY_RELEASE_NO < 30000

#   define MRBX_IMPLANT_VALUE(TT, V)                                    \
           {                                                            \
             .value.ttt = MRBX_BOXNAN_IMPLANT_TYPE(TT, V) >> 32,        \
             .value.i = (V)                                             \
           }                                                            \

#  else

#   define MRBX_IMPLANT_VALUE(TT, V)                                    \
           { .u = MRBX_BOXNAN_IMPLANT_TYPE(TT, V) }                     \

#  endif

# endif

#elif defined(MRB_WORD_BOXING)

# define MRBX_BOXWORD_MAKE_FIXNUM(V)                                    \
         (((unsigned long)(V) << BOXWORD_FIXNUM_SHIFT) |                \
          BOXWORD_FIXNUM_FLAG)                                          \

# define MRBX_BOXWORD_MAKE(TT, V)                                       \
         ((TT) == MRB_TT_FIXNUM ? MRBX_BOXWORD_MAKE_FIXNUM(V) :         \
          (TT) == MRB_TT_UNDEF ? (unsigned long)MRB_Qundef :            \
          (TT) == MRB_TT_TRUE ? (unsigned long)MRB_Qtrue :              \
          (TT) == MRB_TT_FALSE && (V) != 0 ? (unsigned long)MRB_Qfalse : \
          (unsigned long)MRB_Qnil)                                      \

# ifdef __cplusplus

struct mrbx_implant_value
{
  union {
    unsigned long w;
    mrb_value value;
  };

  mrbx_implant_value(enum mrb_vtype tt, mrb_int v) :
    w(MRBX_BOXWORD_MAKE(tt, v))
  {
  }

  operator mrb_value(void) const
  {
    return value;
  }
};

#  define MRBX_IMPLANT_VALUE(TT, V)                                     \
          ((mrb_value)mrbx_implant_value(TT, V))                        \

# else

#  define MRBX_IMPLANT_VALUE(TT, V)                                     \
          { .w = MRBX_BOXWORD_MAKE(TT, V) }                             \

# endif

#else

# ifdef __cplusplus

struct mrbx_implant_value
{
  union {
    struct {
      union {
        mrb_int i;
        mrb_float f;
      };
      enum mrb_vtype tt;
    };
    mrb_value value;
  };

  mrbx_implant_value(enum mrb_vtype tt, mrb_int v) :
    i(v),
    tt(tt)
  {
  }

  operator mrb_value(void) const
  {
    return value;
  }
};

#  define MRBX_IMPLANT_VALUE(TT, V)                                     \
          ((mrb_value)mrbx_implant_value(TT, V))                        \

# else

#  define MRBX_IMPLANT_VALUE(TT, V)                                     \
          { .tt = (TT), .value.i = (V) }                                \

# endif

#endif

#define MRBX_UNDEF_VALUE()      MRBX_IMPLANT_VALUE(MRB_TT_UNDEF, 0)
#define MRBX_NIL_VALUE()        MRBX_IMPLANT_VALUE(MRB_TT_FALSE, 0)
#define MRBX_FALSE_VALUE()      MRBX_IMPLANT_VALUE(MRB_TT_FALSE, 1)
#define MRBX_TRUE_VALUE()       MRBX_IMPLANT_VALUE(MRB_TT_TRUE, 1)
#define MRBX_FIXNUM_VALUE(N)    MRBX_IMPLANT_VALUE(MRB_TT_FIXNUM, N)

MRB_API const char *mrbx_typename(enum mrb_vtype tt);

#endif /* MRUBY_AUX_VALUE_H */
