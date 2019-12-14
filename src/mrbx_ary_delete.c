#include <mruby-aux/array.h>
#include <mruby-aux/compat/array.h>

MRB_API mrb_value
mrbx_ary_delete(mrb_state *mrb, struct RArray *ary, mrb_value obj)
{
  mrb_value *p = (mrb_value *)ARY_PTR(ary);
  mrb_int len = ARY_LEN(ary);
  mrb_int cur = 0;
  for (mrb_int i = 0; i < len; i ++) {
    if (mrb_equal(mrb, obj, p[i])) {
      continue;
    } else {
      if (i > cur) { p[cur] = p[i]; }
      cur ++;
    }
  }

  if (cur < len) {
    ARY_SET_LEN(ary, cur);
    return obj;
  } else {
    return mrb_nil_value();
  }
}
