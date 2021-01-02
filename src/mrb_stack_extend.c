#include <mruby-aux/compat/mruby.h>
#include <mruby-aux/compat/proc.h>
#include "extend-space.h"

#if MRUBY_RELEASE_NO < 20000
# ifndef MRB_STACK_MAX
#  define MRB_STACK_MAX         0x40000L
# endif

# ifndef MRB_STACK_GROWTH
#  define MRB_STACK_GROWTH      128
# endif

# define EXTEND_STACKS(TOKEN)   EXTEND_STACK_ ## TOKEN
# define EXTEND_STACK_BASE      c->stbase
# define EXTEND_STACK_END       c->stend
# define EXTEND_STACK_POINTER   c->stack
# define EXTEND_STACK_ROOM      room
# define EXTEND_STACK_TYPE      mrb_value
# define EXTEND_STACK_MAX       MRB_STACK_MAX

static size_t
EXTEND_STACK_EXTEND(mrb_state *mrb, size_t n, size_t room)
{
  size_t pivot = n + room;
  // TODO: check the overflow

# ifdef MRB_STACK_EXTEND_DOUBLING
  while (n < pivot) {
    n *= 2;
  }
# else
  n = ((pivot + (MRB_STACK_GROWTH - 1)) / MRB_STACK_GROWTH) * MRB_STACK_GROWTH;
# endif

  if (n < pivot || n > EXTEND_STACK_MAX) {
    mrb_raise(mrb, E_RANGE_ERROR, "reached extend limits");
  }

  return n;
}

MRB_API void
mrb_stack_extend(mrb_state *mrb, mrb_int room)
{
  struct mrb_context *c = mrb->c;
  mrb_value *base = c->stbase;

  EXTEND_SPACE(EXTEND_STACKS);

  if (base != c->stbase) {
    ptrdiff_t offset = c->stbase - base;
    mrb_callinfo *ci = c->ci;
    size_t i;

    for (i = (c->ci + 1) - c->cibase; i > 0; i--, ci++) {
      ci->stackent += offset;

      struct REnv *e = ci->env;
      if (e && MRB_ENV_STACK_SHARED_P(e) && e->stack) {
        e->stack += offset;
      }

      struct RProc *p = ci->proc;
      struct REnv *pe = p ? MRB_PROC_ENV(p) : NULL;
      if (pe != e && MRB_ENV_STACK_SHARED_P(pe) && pe->stack) {
        pe->stack += offset;
      }
    }
  }
}
#endif /* MRUBY_RELEASE_NO < 20000 */
