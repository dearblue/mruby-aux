#include <mruby-aux/fakedin.h>
#include <mruby-aux/class.h>
#include <mruby/class.h>

static mrb_value
fakedin_s_new(mrb_state *mrb, mrb_value self)
{
  mrb_value stream;
  mrb_get_args(mrb, "o", &stream);

  return mrbx_obj_extend(mrb, mrbx_fakedin_new(mrb, stream), mrb_class_ptr(self));
}

static mrb_value
fakedin_read(mrb_state *mrb, mrb_value self)
{
  mrb_int size;
  mrb_get_args(mrb, "i", &size);

  const char *buf;
  size = mrbx_fakedin_read(mrb, self, &buf, size);
  if (size < 0) {
    return mrb_nil_value();
  }

  return mrb_str_new(mrb, buf, size);
}

static mrb_value
fakedin_eof(mrb_state *mrb, mrb_value self)
{
  mrb_get_args(mrb, "");

  return mrb_bool_value(mrbx_fakedin_eof(mrb, self));
}

static mrb_value
fakedin_tell(mrb_state *mrb, mrb_value self)
{
  mrb_get_args(mrb, "");

  return mrb_fixnum_value(mrbx_fakedin_total_in(mrb, self));
}

void
mruby_aux_test_fakedin_init(mrb_state *mrb, struct RClass *test)
{
  struct RClass *fakedin = mrb_define_module_under(mrb, test, "FakedIn");
  mrb_define_class_method(mrb, fakedin, "new", fakedin_s_new, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, fakedin, "read", fakedin_read, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, fakedin, "eof?", fakedin_eof, MRB_ARGS_NONE());
  mrb_define_method(mrb, fakedin, "tell", fakedin_tell, MRB_ARGS_NONE());
}
