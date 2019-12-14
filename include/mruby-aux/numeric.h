#ifndef MRUBY_AUX_NUMERIC_H
#define MRUBY_AUX_NUMERIC_H 1

#include <mruby.h>
#include <mruby/numeric.h>

/*
 * Decide the behavior in case of lack of numerical precision
 */
enum mrbx_num_behavior {
  /* Raise a RangeError exception */
  MRBX_NUM_RAISE = 0,

  /* Return nil instead of numeric */
  MRBX_NUM_RETURN_NIL = 1,

  /* Use lower bits of value */
  MRBX_NUM_USE_LOWER_BIT = 2,

  /* Force convert to Float; same as MRBX_NUM_USE_LOWER_BIT with MRB_WITHOUT_FLOAT */
  MRBX_NUM_DROP_PRECISION = 3,
};

/*
 * Convert to Fixnum/Float from int64_t/uint64_t.
 */
MRB_API mrb_value mrbx_num_from_int64(mrb_state *mrb, int64_t n, enum mrbx_num_behavior behavior);
MRB_API mrb_value mrbx_num_from_uint64(mrb_state *mrb, uint64_t n, enum mrbx_num_behavior behavior);

#ifndef MRB_WITHOUT_FLOAT
# include <stdint.h>
# include <float.h>

# define MRBX_FLT_RADIX         FLT_RADIX
# define MRBX_FLT_ROUNDS        FLT_ROUNDS
  /* c99 feature */
# ifdef FLT_EVAL_METHOD
#  define MRBX_FLT_EVAL_METHOD  FLT_EVAL_METHOD
# endif

# ifdef MRB_USE_FLOAT
#  define MRBX_FLT_MANT_DIG     FLT_MANT_DIG
#  define MRBX_FLT_EPSILON      FLT_EPSILON
#  define MRBX_FLT_DIG          FLT_DIG
#  define MRBX_FLT_MIN_EXP      FLT_MIN_EXP
#  define MRBX_FLT_MIN          FLT_MIN
#  define MRBX_FLT_MIN_10_EXP   FLT_MIN_10_EXP
#  define MRBX_FLT_MAX_EXP      FLT_MAX_EXP
#  define MRBX_FLT_MAX          FLT_MAX
#  define MRBX_FLT_MAX_10_EXP   FLT_MAX_10_EXP
   /* c11 features */
#  ifdef FLT_TRUE_MIN
#   define MRBX_FLT_TRUE_MIN    FLT_TRUE_MIN
#  endif
#  ifdef FLT_DECIMAL_DIG
#   define MRBX_FLT_DECIMAL_DIG FLT_DECIMAL_DIG
#  endif
#  ifdef FLT_HAS_SUBNORM
#   define MRBX_FLT_HAS_SUBNORM FLT_HAS_SUBNORM
#  endif

# else /* not MRB_USE_FLOAT */
#  define MRBX_FLT_MANT_DIG     DBL_MANT_DIG
#  define MRBX_FLT_EPSILON      DBL_EPSILON
#  define MRBX_FLT_DIG          DBL_DIG
#  define MRBX_FLT_MIN_EXP      DBL_MIN_EXP
#  define MRBX_FLT_MIN          DBL_MIN
#  define MRBX_FLT_MIN_10_EXP   DBL_MIN_10_EXP
#  define MRBX_FLT_MAX_EXP      DBL_MAX_EXP
#  define MRBX_FLT_MAX          DBL_MAX
#  define MRBX_FLT_MAX_10_EXP   DBL_MAX_10_EXP
   /* c11 features */
#  ifdef FLT_TRUE_MIN
#   define MRBX_FLT_TRUE_MIN    DBL_TRUE_MIN
#  endif
#  ifdef FLT_DECIMAL_DIG
#   define MRBX_FLT_DECIMAL_DIG DBL_DECIMAL_DIG
#  endif
#  ifdef FLT_HAS_SUBNORM
#   define MRBX_FLT_HAS_SUBNORM DBL_HAS_SUBNORM
#  endif
# endif /* MRB_USE_FLOAT */

/* Maximum/minimum real number that can keeps integer precision */
# define MRBX_FLT_INT_MIN       ((int64_t)((uint64_t)-1 << MRBX_FLT_MANT_DIG))
# define MRBX_FLT_INT_MAX       ((int64_t)((uint64_t)+1 << MRBX_FLT_MANT_DIG))
#endif /* MRB_WITHOUT_FLOAT */

#endif /* MRUBY_AUX_NUMERIC_H */
