#include <mruby-aux/string.h>

static mrb_value
test_str_drop(mrb_state *mrb, mrb_value self)
{
  mrb_value str;
  mrb_int off, size;
  mrb_get_args(mrb, "Sii", &str, &off, &size);

  return mrb_obj_value(mrbx_str_drop(mrb, RSTRING(str), off, size));
}

void
mruby_aux_test_string_init(mrb_state *mrb, struct RClass *test)
{
  mrb_define_class_method(mrb, test, "str_drop", test_str_drop, MRB_ARGS_REQ(3));
}
