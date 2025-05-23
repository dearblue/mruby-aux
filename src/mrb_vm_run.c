#include <mruby-aux/compat/mruby.h>

#if MRUBY_RELEASE_NO < 10300
MRB_API mrb_value
mrb_vm_run(mrb_state *mrb, const struct RProc *proc, mrb_value self, unsigned int stack_keep)
{
  return mrb_context_run(mrb, (struct RProc *)(uintptr_t)proc, self, stack_keep);
}
#endif
