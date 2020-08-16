#include <mruby-aux/compat/mruby.h>
#include <mruby-aux/proc.h>

MRB_API void
mrbx_method_extract(mrb_state *mrb, mrb_method_t m, int argc, struct RProc **proc, mrb_func_t *cfunc)
{
  if (MRB_METHOD_UNDEF_P(m)) {
    mrb_raise(mrb, E_NOMETHOD_ERROR, "undefined method");
  }

  if (argc != 0 && MRB_METHOD_NOARG_P(m)) {
    mrb_argnum_error(mrb, argc, 0, 0);
  }

  if (MRB_METHOD_PROC_P(m)) {
    *proc = MRB_METHOD_PROC(m);
    mrb_assert(*proc != NULL);

    if (MRB_PROC_CFUNC_P(*proc)) {
      *cfunc = MRBX_PROC_CFUNC(*proc);
      mrb_assert(*cfunc != NULL);
    } else {
      *cfunc = NULL;
    }
  } else {
    *proc = NULL;
    *cfunc = MRB_METHOD_FUNC(m);
    mrb_assert(*cfunc != NULL);
  }
}
