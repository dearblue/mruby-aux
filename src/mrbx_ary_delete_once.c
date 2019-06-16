#include <mruby-aux/array.h>
#include <mruby-aux/compat/array.h>

MRB_API mrb_value
mrbx_ary_delete_once(mrb_state *mrb, struct RArray *ary, mrb_value obj)
{
  const mrb_value *p = ARY_PTR(ary);
  mrb_int len = ARY_LEN(ary);
  for (mrb_int i = 0; i < len; i ++) {
    if (mrb_equal(mrb, obj, p[i])) {
      mrbx_ary_delete_at(mrb, ary, i);
      return obj;
    }
  }

  return mrb_nil_value();
}
