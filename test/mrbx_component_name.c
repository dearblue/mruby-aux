#include <mruby.h>
#include <mruby/array.h>
#include <mruby-aux.h>
#include <mruby-aux/component-name.h>

static mrb_value
test_need_pathsep_p(mrb_state *mrb, mrb_value self)
{
  const char *path;
  mrb_get_args(mrb, "z", &path);
  size_t len = strlen(path);
  if (len > UINT16_MAX) {
    mrb_raise(mrb, E_RANGE_ERROR, "out of path length");
  }

  return mrb_bool_value(mrbx_need_pathsep_p(path, len));
}

static mrb_value
test_pathsep_p(mrb_state *mrb, mrb_value self)
{
  const char *ch;
  mrb_get_args(mrb, "z", &ch);
  size_t len = strlen(ch);
  if (len != 1) {
    mrb_raise(mrb, E_RANGE_ERROR, "need one character");
  }

  return mrb_bool_value(mrbx_pathsep_p((int)(unsigned char)ch[0]));
}

static mrb_value
test_split_path(mrb_state *mrb, mrb_value self)
{
  const char *path;
  mrb_get_args(mrb, "z", &path);
  size_t len = strlen(path);
  if (len > UINT16_MAX) {
    mrb_raise(mrb, E_RANGE_ERROR, "out of path length");
  }

  mrbx_component_name cn = mrbx_split_path(path, len);
  mrb_value ret = mrb_ary_new_capa(mrb, 5);
  mrb_ary_push(mrb, ret, mrb_fixnum_value(cn.rootterm));
  mrb_ary_push(mrb, ret, mrb_fixnum_value(cn.dirterm));
  mrb_ary_push(mrb, ret, mrb_fixnum_value(cn.basename));
  mrb_ary_push(mrb, ret, mrb_fixnum_value(cn.extname));
  mrb_ary_push(mrb, ret, mrb_fixnum_value(cn.nameterm));

  return ret;
}

void
mruby_aux_test_component_name_init(mrb_state *mrb, struct RClass *test)
{
  mrb_define_class_method(mrb, test, "need_pathsep?", test_need_pathsep_p, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, test, "pathsep?", test_pathsep_p, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, test, "split_path", test_split_path, MRB_ARGS_REQ(1));
}
