#include <mruby-aux/compat/mruby.h>
#include <mruby-aux/vmext.h>

mrb_value *
mrbx_vm_stack_extend(mrb_state *mrb, mrb_int room)
{
  mrb_stack_extend(mrb, room);
  return mrbx_vm_top_stacks(mrb->c);
}
