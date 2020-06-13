#include <mruby-aux/vmext.h>
#include "extend-space.h"

# define EXTEND_CI(TOKEN)       EXTEND_CI_ ## TOKEN
# define EXTEND_CI_BASE         c->cibase
# define EXTEND_CI_END          c->ciend
# define EXTEND_CI_POINTER      c->ci
# define EXTEND_CI_ROOM         room
# define EXTEND_CI_TYPE         mrb_callinfo
# define EXTEND_CI_MAX          (UINT32_C(1) << 20)
# define EXTEND_CI_EXTEND       extend_ci_extend

static size_t
extend_ci_extend(mrb_state *mrb, size_t n, size_t room)
{
  size_t pivot = n + room;
  // TODO: check the overflow

  while (n < pivot) {
    n *= 2;
  }

  if (n < pivot || n > EXTEND_CI_MAX) {
    mrb_raise(mrb, E_RANGE_ERROR, "reached extend limits");
  }

  return n;
}

mrb_callinfo *
mrbx_vm_ciextend(mrb_state *mrb, size_t room)
{
  struct mrb_context *c = mrb->c;

  EXTEND_SPACE(EXTEND_CI);

  return c->ci;
}
