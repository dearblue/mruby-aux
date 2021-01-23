#include <mruby-aux.h>
#include <mruby-aux/utils.h>

MRB_API void
mrbx_get_read_args(mrb_state *mrb, ssize_t *size, struct RString **dest)
{
  mrb_value sizev = mrb_nil_value();
  mrb_value bufv = sizev;
  mrb_int argc = mrb_get_args(mrb, "|oS!", &sizev, &bufv);

  if (argc == 0) {
    *size = -1;
    *dest = NULL;
  } else {
    if (NIL_P(sizev)) {
      *size = -1;
    } else {
      if (mrb_float_p(sizev)) {
        mrb_float n = mrb_float(sizev);
        if (MRBX_FLOAT_OUT_OF_INTEGER_P(n)) {
          mrb_raisef(mrb, E_RANGE_ERROR,
                     "missing precision for Integer from Float - %S",
                     sizev);
        }
        *size = n;
      } else {
        *size = mrb_int(mrb, sizev);
      }

      if (*size < 0) {
        mrb_raisef(mrb, E_RANGE_ERROR,
                   "wrong negative value for size - %S",
                   sizev);
      }
    }

    *dest = (mrb_nil_p(bufv) ? NULL : RSTRING(bufv));
  }

  *dest = mrbx_str_force_recycle(mrb, *dest, (*size < 0 ? 0 : *size));
  RSTR_SET_LEN((*dest), 0);
}
