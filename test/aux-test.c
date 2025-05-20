#include <mruby.h>

void mruby_aux_test_array_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_component_name_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_fakedin_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_scanhash_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_string_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_value_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_vm_call_super_init(mrb_state *mrb, struct RClass *test);

static mrb_value
auxtest_s_windows_p(mrb_state *mrb, mrb_value self)
{
#ifdef _WIN32
  return mrb_true_value();
#else
  return mrb_false_value();
#endif
}

void
mrb_mruby_aux_gem_test(mrb_state *mrb)
{
  struct RClass *mAuxTest = mrb_define_module(mrb, "AuxTest");
  mrb_define_class_method(mrb, mAuxTest, "windows?", auxtest_s_windows_p, MRB_ARGS_NONE());

  mruby_aux_test_array_init(mrb, mAuxTest);
  mruby_aux_test_component_name_init(mrb, mAuxTest);
  mruby_aux_test_fakedin_init(mrb, mAuxTest);
  mruby_aux_test_scanhash_init(mrb, mAuxTest);
  mruby_aux_test_string_init(mrb, mAuxTest);
  mruby_aux_test_value_init(mrb, mAuxTest);
  mruby_aux_test_vm_call_super_init(mrb, mAuxTest);
}
