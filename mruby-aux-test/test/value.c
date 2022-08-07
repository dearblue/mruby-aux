#include <mruby.h>
#include <mruby-aux/value.h>

static mrb_value
value_s_check_cptr(mrb_state *mrb, mrb_value mod)
{
  static void *const ptrs[] = {
    (void*)0, (void*)1, (void*)2, (void*)3,
    (void*)4, (void*)5, (void*)6, (void*)7,
    (void*)8, (void*)9, (void*)10, (void*)11,
    (void*)12, (void*)13, (void*)14, (void*)15,
#if MRBX_MRUBY_RELEASE_NO < 30001 || !defined(MRB_NAN_BOXING)
    (void*)-1, (void*)-2, (void*)-3, (void*)-4,
    (void*)-5, (void*)-6, (void*)-7, (void*)-8,
    (void*)-9, (void*)-10, (void*)-11, (void*)-12,
    (void*)-13, (void*)-14, (void*)-15, (void*)-16
#endif
  };

  for (size_t i = 0; i < sizeof(ptrs) / sizeof(ptrs[0]); i++) {
    mrb_value a = mrb_cptr_value(mrb, ptrs[i]);
    if (mrb_cptr(a) != ptrs[i]) { return mrb_fixnum_value(i); }
  }

  return mrb_true_value();
}

static mrb_value
value_s_check_undef(mrb_state *mrb, mrb_value mod)
{
  static const mrb_value v = MRBX_UNDEF_VALUE();
#if !defined(MRB_WORD_BOXING) && (MRBX_MRUBY_RELEASE_NO < 30001 || !defined(MRB_NAN_BOXING))
  if (mrb_fixnum(v) != 0) { return mrb_false_value(); }
#endif
  return mrb_bool_value(mrb_undef_p(v));
}

static mrb_value
value_s_check_nil(mrb_state *mrb, mrb_value mod)
{
  static const mrb_value v = MRBX_NIL_VALUE();
#if !defined(MRB_WORD_BOXING) && (MRBX_MRUBY_RELEASE_NO < 30001 || !defined(MRB_NAN_BOXING))
  if (mrb_fixnum(v) != 0) { return mrb_false_value(); }
#endif
  return mrb_bool_value(mrb_nil_p(v));
}

static mrb_value
value_s_nil(mrb_state *mrb, mrb_value mod)
{
  static const mrb_value v = MRBX_NIL_VALUE();
  return v;
}

static mrb_value
value_s_check_false(mrb_state *mrb, mrb_value mod)
{
  static const mrb_value v = MRBX_FALSE_VALUE();
#if !defined(MRB_WORD_BOXING) && (MRBX_MRUBY_RELEASE_NO < 30001 || !defined(MRB_NAN_BOXING))
  if (mrb_fixnum(v) != 1) { return mrb_false_value(); }
#endif
  return mrb_bool_value(mrb_false_p(v));
}

static mrb_value
value_s_false(mrb_state *mrb, mrb_value mod)
{
  static const mrb_value v = MRBX_FALSE_VALUE();
  return v;
}

static mrb_value
value_s_check_true(mrb_state *mrb, mrb_value mod)
{
  static const mrb_value v = MRBX_TRUE_VALUE();
#if !defined(MRB_WORD_BOXING) && (MRBX_MRUBY_RELEASE_NO < 30001 || !defined(MRB_NAN_BOXING))
  if (mrb_fixnum(v) != 1) { return mrb_false_value(); }
#endif
  return mrb_bool_value(mrb_true_p(v));
}

static mrb_value
value_s_true(mrb_state *mrb, mrb_value mod)
{
  static const mrb_value v = MRBX_TRUE_VALUE();
  return v;
}

static mrb_value
value_s_check_fixnum(mrb_state *mrb, mrb_value mod)
{
  mrb_value v = MRBX_FIXNUM_VALUE(1);
  if (mrb_fixnum(v) != 1) { return mrb_false_value(); }
  return mrb_bool_value(mrb_fixnum_p(v));
}

static mrb_value
value_s_fixnum(mrb_state *mrb, mrb_value mod)
{
  mrb_int i;
  mrb_get_args(mrb, "i", &i);
  mrb_value v = MRBX_FIXNUM_VALUE(i);
  return v;
}

static const mrb_value fix0 = MRBX_FIXNUM_VALUE(0);
static const mrb_value fix1 = MRBX_FIXNUM_VALUE(1);
static const mrb_value fix1m = MRBX_FIXNUM_VALUE(-1);

void
mruby_aux_test_value_init(mrb_state *mrb, struct RClass *test)
{
  (void)fix0;
  (void)fix1;
  (void)fix1m;

  mrb_define_class_method(mrb, test, "check_cptr", value_s_check_cptr, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, test, "check_undef", value_s_check_undef, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, test, "check_nil", value_s_check_nil, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, test, "nil", value_s_nil, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, test, "check_false", value_s_check_false, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, test, "false", value_s_false, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, test, "check_true", value_s_check_true, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, test, "true", value_s_true, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, test, "check_fixnum", value_s_check_fixnum, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, test, "fixnum", value_s_fixnum, MRB_ARGS_REQ(1));

#if MRUBY_RELEASE_NO > 20101 || !defined(MRB_NAN_BOXING)
# define CHECK_CPTR_SKIP FALSE
#else
# define CHECK_CPTR_SKIP TRUE
#endif

  mrb_define_const(mrb, test, "CHECK_CPTR_SKIP", mrb_bool_value(CHECK_CPTR_SKIP));
}
