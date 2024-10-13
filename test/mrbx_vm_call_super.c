#include <mruby-aux/vmext.h>

static mrb_value
callsuper(mrb_state *mrb, mrb_value self)
{
  mrb_int argc;
  mrb_value *argv;
  mrb_value block;
  mrb_get_args(mrb, "*&", &argv, &argc, &block);

  return mrbx_vm_call_super(mrb, self, argc, argv, block);
}

void
mruby_aux_test_vm_call_super_init(mrb_state *mrb, struct RClass *test)
{
  struct RClass *super1 = mrb_define_class_under(mrb, test, "CallSuper1", mrb->object_class);

  struct RClass *super2 = mrb_define_class_under(mrb, test, "CallSuper2", super1);
  mrb_define_method(mrb, super2, "callsuper", callsuper, MRB_ARGS_ANY());

  struct RClass *super3 = mrb_define_class_under(mrb, test, "CallSuper3", super2);
  mrb_define_method(mrb, super3, "callsuper", callsuper, MRB_ARGS_ANY());
}
