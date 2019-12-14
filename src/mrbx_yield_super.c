#include <mruby-aux/class.h>

MRB_API mrb_value
mrbx_yield_super(mrb_state *mrb, mrb_value self, mrb_int argc, const mrb_value *argv, struct RClass **c)
{
  int ai = mrb_gc_arena_save(mrb);
  mrb_sym mid = mrb_get_mid(mrb);

  if (mid < 1) {
nomethod_err:
    mrb_raise(mrb, E_NOMETHOD_ERROR,
              "super: no superclass method");
  }

  struct RClass *dummy_c;
  if (c == NULL) { dummy_c = NULL; c = &dummy_c; }

  if (*c == NULL) { *c = mrb_class(mrb, self); }

  mrb_method_t m = mrb_method_search_vm(mrb, c, mid);
  if (MRB_METHOD_UNDEF_P(m) || *c == NULL) { goto nomethod_err; }

  *c = (*c)->super;
  if (c == NULL) { goto nomethod_err; }

  m = mrb_method_search_vm(mrb, c, mid);
  if (MRB_METHOD_UNDEF_P(m) || *c == NULL) { goto nomethod_err; }

  mrb_value ret = mrb_yield_with_class(mrb, mrbx_proc_from_method(mrb, m), argc, argv, self, *c);
  mrb_gc_arena_restore(mrb, ai);
  mrb_gc_protect(mrb, ret);

  return ret;
}
