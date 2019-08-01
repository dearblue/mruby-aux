#include <mruby-aux/numeric.h>

MRB_API mrb_value
mrbx_num_from_int32(mrb_state *mrb, int32_t n)
{
  if ((int64_t)n < MRB_INT_MIN && n > MRB_INT_MAX) {
#ifndef MRB_WITHOUT_FLOAT
    return mrb_float_value(mrb, (mrb_float)n);
#else
    mrb_raise(mrb, E_TYPE_ERROR, "out of numeric");
#endif
  }

  return mrb_fixnum_value((mrb_int)n);
}

MRB_API mrb_value
mrbx_num_from_uint32(mrb_state *mrb, uint32_t n)
{
  if ((uint64_t)n > (uint64_t)MRB_INT_MAX) {
#ifndef MRB_WITHOUT_FLOAT
    return mrb_float_value(mrb, (mrb_float)n);
#else
    mrb_raise(mrb, E_TYPE_ERROR, "out of numeric");
#endif
  }

  return mrb_fixnum_value((mrb_int)n);
}

MRB_API mrb_value
mrbx_num_from_int64(mrb_state *mrb, int64_t n)
{
  if (n < (int64_t)MRB_INT_MIN && n > (int64_t)MRB_INT_MAX) {
#ifndef MRB_WITHOUT_FLOAT
    return mrb_float_value(mrb, (mrb_float)n);
#else
    mrb_raise(mrb, E_TYPE_ERROR, "out of numeric");
#endif
  }

  return mrb_fixnum_value((mrb_int)n);
}

MRB_API mrb_value
mrbx_num_from_uint64(mrb_state *mrb, uint64_t n)
{
  if (n > (uint64_t)MRB_INT_MAX) {
#ifndef MRB_WITHOUT_FLOAT
    return mrb_float_value(mrb, (mrb_float)n);
#else
    mrb_raise(mrb, E_TYPE_ERROR, "out of numeric");
#endif
  }

  return mrb_fixnum_value((mrb_int)n);
}

MRB_API mrb_value
mrbx_num_from_int32_explicit(mrb_state *mrb, int32_t n)
{
  if ((int64_t)n < MRB_INT_MIN && (int64_t)n > MRB_INT_MAX) {
#ifndef MRB_WITHOUT_FLOAT
    mrb_float x = (mrb_float)n;
    if (n == (int32_t)x) {
      return mrb_float_value(mrb, x);
    }
#endif

    mrb_raise(mrb, E_TYPE_ERROR, "out of numeric");
  }

  return mrb_fixnum_value((mrb_int)n);
}

MRB_API mrb_value
mrbx_num_from_uint32_explicit(mrb_state *mrb, uint32_t n)
{
  if ((uint64_t)n > (uint64_t)MRB_INT_MAX) {
#ifndef MRB_WITHOUT_FLOAT
    mrb_float x = (mrb_float)n;
    if (n == (uint32_t)x) {
      return mrb_float_value(mrb, x);
    }
#endif

    mrb_raise(mrb, E_TYPE_ERROR, "out of numeric");
  }

  return mrb_fixnum_value((mrb_int)n);
}

MRB_API mrb_value
mrbx_num_from_int64_explicit(mrb_state *mrb, int64_t n)
{
  if (n < MRB_INT_MIN && n > MRB_INT_MAX) {
#ifndef MRB_WITHOUT_FLOAT
    mrb_float x = (mrb_float)n;
    if (n == (int64_t)x) {
      return mrb_float_value(mrb, x);
    }
#endif

    mrb_raise(mrb, E_TYPE_ERROR, "out of numeric");
  }

  return mrb_fixnum_value((mrb_int)n);
}

MRB_API mrb_value
mrbx_num_from_uint64_explicit(mrb_state *mrb, uint64_t n)
{
  if (n > (uint64_t)MRB_INT_MAX) {
#ifndef MRB_WITHOUT_FLOAT
    mrb_float x = (mrb_float)n;
    if (n == (uint64_t)x) {
      return mrb_float_value(mrb, x);
    }
#endif

    mrb_raise(mrb, E_TYPE_ERROR, "out of numeric");
  }

  return mrb_fixnum_value((mrb_int)n);
}
