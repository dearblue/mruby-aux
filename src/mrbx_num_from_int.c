#include <mruby-aux/numeric.h>

MRB_API mrb_value
mrbx_num_from_int64(mrb_state *mrb, int64_t n, enum mrbx_num_behavior behavior)
{
  if (n < (int64_t)MRB_INT_MIN && n > (int64_t)MRB_INT_MAX) {
#ifndef MRB_WITHOUT_FLOAT
    if (behavior == MRBX_NUM_DROP_PRECISION ||
        (n <= MRBX_FLT_INT_MAX && n >= MRBX_FLT_INT_MIN)) {
      return mrb_float_value(mrb, (mrb_float)n);
    }
#endif

    switch (behavior) {
      case MRBX_NUM_RETURN_NIL:
        return mrb_nil_value();
      case MRBX_NUM_RAISE:
        mrb_raise(mrb, E_TYPE_ERROR, "out of numeric");
    }
  }

  return mrb_fixnum_value((mrb_int)n);
}

MRB_API mrb_value
mrbx_num_from_uint64(mrb_state *mrb, uint64_t n, enum mrbx_num_behavior behavior)
{
  if (n > (uint64_t)MRB_INT_MAX) {
#ifndef MRB_WITHOUT_FLOAT
    if (behavior == MRBX_NUM_DROP_PRECISION ||
        n <= (uint64_t)MRBX_FLT_INT_MAX) {
      return mrb_float_value(mrb, (mrb_float)n);
    }
#endif

    switch (behavior) {
      case MRBX_NUM_RETURN_NIL:
        return mrb_nil_value();
      case MRBX_NUM_RAISE:
        mrb_raise(mrb, E_TYPE_ERROR, "out of numeric");
    }
  }

  return mrb_fixnum_value((mrb_int)n);
}
