#ifndef MRUBY_AUX_VALUE_H
#define MRUBY_AUX_VALUE_H 1

#include "common.h"
#include "compat/value.h"

/* The implant immediate values */

#if defined(MRB_NAN_BOXING)
# ifdef __cplusplus

struct mrbx_implant_value
{
  union {
    double f;
    uint64_t w;
  };

  mrbx_implant_value(enum mrb_vtype tt, int64_t v) :
    w(UINT64_C(0xfff0000000000000) |
      (((tt + UINT64_C(1)) & 0x3f) << 46) |
      (uint64_t(v) & UINT64_C(0x00003fffffffffff)))
  {
  }

  operator const mrb_value *(void) const
  {
    return (const mrb_value *)this;
  }
};

#  define MRBX_IMPLANT_VALUE(TT, V)                                     \
        (*(const mrb_value *)mrbx_implant_value(TT, (mrb_int)(V)))      \

# else

#  define MRBX_BOXNAN_IMPLANT_TYPE(TT, HI)                              \
        (                                                               \
          UINT32_C(0xfff00000) |                                        \
          ((((TT) + UINT32_C(1)) & 0x3f) << 14) |                       \
          ((HI) & UINT16_C(0x3fff))                                     \
        )                                                               \

#  define MRBX_IMPLANT_VALUE(TT, V)                                     \
        { .value.ttt = MRBX_BOXNAN_IMPLANT_TYPE(TT, V), .value.i = (V) } \

# endif

#elif defined(MRB_WORD_BOXING)

# define MRBX_BOXWORD_MAKE_FIXNUM(V)                                    \
        (((unsigned long)(V) << BOXWORD_FIXNUM_SHIFT) | BOXWORD_FIXNUM_FLAG) \

# define MRBX_BOXWORD_MAKE(TT, V)                                       \
        (                                                               \
          (TT) == MRB_TT_FIXNUM ? MRBX_BOXWORD_MAKE_FIXNUM(V) :         \
          (TT) == MRB_TT_UNDEF ? (unsigned long)MRB_Qundef :            \
          (TT) == MRB_TT_TRUE ? (unsigned long)MRB_Qtrue :              \
          (TT) == MRB_TT_FALSE && (V) != 0 ? (unsigned long)MRB_Qfalse : \
          (unsigned long)MRB_Qnil                                       \
        )                                                               \

# ifdef __cplusplus

struct mrbx_implant_value
{
  unsigned long w;

  mrbx_implant_value(enum mrb_vtype tt, mrb_int v) :
    w(MRBX_BOXWORD_MAKE(tt, v))
  {
  }

  operator const mrb_value *(void) const
  {
    return (const mrb_value *)this;
  }
};

#  define MRBX_IMPLANT_VALUE(TT, V)                                     \
        (*(const mrb_value *)mrbx_implant_value(TT, V))                 \

# else

#  define MRBX_IMPLANT_VALUE(TT, V)                                     \
        { .w = MRBX_BOXWORD_MAKE(TT, V) }                               \

# endif

#else

# ifdef __cplusplus

struct mrbx_implant_value
{
  union {
    mrb_int i;
    mrb_float f;
  };
  enum mrb_vtype tt;

  mrbx_implant_value(enum mrb_vtype tt, mrb_int v) :
    i(v),
    tt(tt)
  {
  }

  operator const mrb_value *(void) const
  {
    return (const mrb_value *)this;
  }
};

#  define MRBX_IMPLANT_VALUE(TT, V)                                     \
        (*(const mrb_value *)mrbx_implant_value(TT, V))                 \

# else

#  define MRBX_IMPLANT_VALUE(TT, V)                                     \
        { .tt = (TT), .value.i = (V) }                                  \

# endif

#endif

#define MRBX_UNDEF_VALUE()      MRBX_IMPLANT_VALUE(MRB_TT_UNDEF, 0)
#define MRBX_NIL_VALUE()        MRBX_IMPLANT_VALUE(MRB_TT_FALSE, 0)
#define MRBX_FALSE_VALUE()      MRBX_IMPLANT_VALUE(MRB_TT_FALSE, 1)
#define MRBX_TRUE_VALUE()       MRBX_IMPLANT_VALUE(MRB_TT_TRUE, 1)
#define MRBX_FIXNUM_VALUE(N)    MRBX_IMPLANT_VALUE(MRB_TT_FIXNUM, N)

MRB_API const char *mrbx_typename(enum mrb_vtype tt);

#endif /* MRUBY_AUX_VALUE_H */
