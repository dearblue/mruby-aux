#include <mruby-aux/array.h>
#include <mruby-aux/compat/array.h>

MRB_API mrb_value
mrbx_ary_delete_at(mrb_state *mrb, struct RArray *ary, mrb_int idx)
{
  mrb_int len = ARY_LEN(ary);
  if (idx < 0) { idx += len; }
  if (idx < 0 || idx >= len) { return mrb_nil_value(); }

  mrb_value ret = ARY_PTR(ary)[idx];

  mrb_int pivot = idx + 1;
  if (pivot < len) {
    mrb_value *p = (mrb_value *)ARY_PTR(ary);
    memmove(p + idx, p + pivot, sizeof(mrb_value) * (len - pivot));
  }

  ARY_SET_LEN(ary, len - 1);

  return ret;
}
