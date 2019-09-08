#include <mruby-aux/class.h>

MRB_API mrb_value
mrbx_obj_extend(mrb_state *mrb, mrb_value o, struct RClass *mod)
{
  if (mod->tt != MRB_TT_MODULE) {
    mrb_raise(mrb, E_TYPE_ERROR, "type mismatch (expect module)");
  }

  if (!mrb_obj_is_kind_of(mrb, o, mod)) {
    mrb_include_module(mrb, mrb_class_ptr(mrb_singleton_class(mrb, o)), mod);
    mrb_funcall_argv(mrb, mrb_obj_value(mod), mrb_intern_lit(mrb, "extended"), 1, &o);
  }

  return o;
}
