#include <mruby-aux/proc.h>

#define STR_METHOD_MISSING      "method_missing"
#define ID_METHOD_MISSING       mrb_intern_lit(mrb, STR_METHOD_MISSING)

MRB_API mrb_sym
mrbx_resolve_method_missing(mrb_state *mrb, struct RClass **target_class, struct RProc **proc, mrb_func_t *cfunc)
{
  mrb_sym mid = ID_METHOD_MISSING;
  mrb_method_t m = mrb_method_search_vm(mrb, target_class, mid);

  if (MRB_METHOD_UNDEF_P(m)) {
    mrb_raise(mrb, E_NOMETHOD_ERROR, "undefined method '" STR_METHOD_MISSING "'");
  }

  mrbx_method_extract(mrb, m, -1, proc, cfunc);

  return mid;
}
