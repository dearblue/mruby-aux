#include <mruby.h>
#include <mruby/array.h>
#include <mruby-aux.h>
#include <mruby-aux/pathinfo.h>

static mrb_value
test_path_normalize(mrb_state *mrb, mrb_value self)
{
  mrb_value path;
  mrb_get_args(mrb, "S", &path);
  path = mrb_str_dup(mrb, path);
  mrb_string_cstr(mrb, path);

  struct RString *s = mrb_str_ptr(path);
  mrb_str_modify(mrb, s);

  const char *end = RSTR_PTR(s) + RSTR_LEN(s);
  mrbx_path_normalize(RSTR_PTR(s), &end);
  RSTR_SET_LEN(s, end - RSTR_PTR(s));

  return path;
}

static mrb_value
test_path_need_separator_p(mrb_state *mrb, mrb_value self)
{
  const char *path;
  mrb_get_args(mrb, "z", &path);
  size_t len = strlen(path);
  if (len > UINT16_MAX) {
    mrb_raise(mrb, E_RANGE_ERROR, "out of path length");
  }

  return mrb_bool_value(mrbx_path_need_separator_p(path, path + len));
}

static mrb_value
test_path_separator_p(mrb_state *mrb, mrb_value self)
{
  const char *ch;
  mrb_get_args(mrb, "z", &ch);
  size_t len = strlen(ch);
  if (len != 1) {
    mrb_raise(mrb, E_RANGE_ERROR, "need one character");
  }

  return mrb_bool_value(mrbx_path_separator_p((unsigned char)ch[0]));
}

static mrb_value
test_pathinfo_parse(mrb_state *mrb, mrb_value self)
{
  const char *path;
  mrb_get_args(mrb, "z", &path);
  size_t len = strlen(path);
  if (len > UINT16_MAX) {
    mrb_raise(mrb, E_RANGE_ERROR, "out of path length");
  }

  mrbx_pathinfo pi = { path, (uint16_t)len };
  mrbx_pathinfo_parse(&pi);
  mrb_value ret = mrb_ary_new_capa(mrb, 5);
  mrb_ary_push(mrb, ret, mrb_fixnum_value(pi.rootterm));
  mrb_ary_push(mrb, ret, mrb_fixnum_value(pi.dirterm));
  mrb_ary_push(mrb, ret, mrb_fixnum_value(pi.basename));
  mrb_ary_push(mrb, ret, mrb_fixnum_value(pi.extname));
  mrb_ary_push(mrb, ret, mrb_fixnum_value(pi.nameterm));

  return ret;
}

void
mruby_aux_test_pathinfo_init(mrb_state *mrb, struct RClass *test)
{
  mrb_define_class_method(mrb, test, "path_normalize", test_path_normalize, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, test, "path_need_separator?", test_path_need_separator_p, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, test, "path_separator?", test_path_separator_p, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, test, "pathinfo_parse", test_pathinfo_parse, MRB_ARGS_REQ(1));
}
