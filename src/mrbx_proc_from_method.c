#include <mruby-aux/class.h>

MRB_API mrb_value
mrbx_proc_from_method(mrb_state *mrb, mrb_method_t m)
{
  const struct RProc *proc;

  if (MRB_METHOD_FUNC_P(m)) {
    proc = mrb_proc_new_cfunc(mrb, MRB_METHOD_FUNC(m));
  } else {
    proc = MRB_METHOD_PROC(m);
  }

  return mrb_obj_value((void *)proc);
}
