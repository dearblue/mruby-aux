#include <mruby.h>

void mruby_aux_test_array_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_fakedin_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_scanhash_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_string_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_value_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_vm_call_super_init(mrb_state *mrb, struct RClass *test);

void
mrb_mruby_aux_test_gem_test(mrb_state *mrb)
{
  struct RClass *mAuxTest = mrb_define_module(mrb, "AuxTest");

  mruby_aux_test_array_init(mrb, mAuxTest);
  mruby_aux_test_fakedin_init(mrb, mAuxTest);
  mruby_aux_test_scanhash_init(mrb, mAuxTest);
  mruby_aux_test_string_init(mrb, mAuxTest);
  mruby_aux_test_value_init(mrb, mAuxTest);
  mruby_aux_test_vm_call_super_init(mrb, mAuxTest);
}
