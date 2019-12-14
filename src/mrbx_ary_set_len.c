#include <mruby-aux/array.h>
#include <mruby-aux/compat/array.h>

MRB_API void
mrbx_ary_set_len(mrb_state *mrb, struct RArray *ary, mrb_int new_len)
{
  if (new_len < 0 || new_len >= ARY_LEN(ary)) {
    mrb_raise(mrb, E_INDEX_ERROR, "out of array");
  }
  ARY_SET_LEN(ary, new_len);
}
