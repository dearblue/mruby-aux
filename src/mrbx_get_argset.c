#include <mruby-aux/utils.h>
#include <mruby/hash.h>
#include <stdbool.h>
#include <mruby-aux/vmext.h>
#include "vm_internals.h"

#if MRBX_MRUBY_RELEASE_NO > 30000
# define ARG_SPLAT_P(ARGC) ((ARGC) >= 15)
# define ARG_KWREST_P(KARGC) ((KARGC) >= 15)
# define NOSPLAT_COUNT_STACK(ARGC) (ARGC)
# define SPLAT_COUNT_STACK 1
# define NOKWREST_COUNT_STACK(KARGC) ((KARGC) * 2)
# define KWREST_COUNT_STACK 1
# define ARG_COUNT_STACK(ARGC) (ARG_SPLAT_P(ARGC) ? SPLAT_COUNT_STACK : NOSPLAT_COUNT_STACK(ARGC))
# define KWARG_COUNT_STACK(KARGC) (ARG_KWREST_P(KARGC) ? KWREST_COUNT_STACK : NOKWREST_COUNT_STACK(KARGC))
# define CI_ARGC_GET(CI) ((CI)->n)
# define CI_ARG_BLOCK_INDEX(CI) (ARG_COUNT_STACK((CI)->n) + KWARG_COUNT_STACK((CI)->nk) + 1 /* self */)

static mrb_value
pack_kwargs(mrb_state *mrb, int nkeys, const mrb_value seq[])
{
  mrb_value hash = mrb_hash_new_capa(mrb, nkeys);
  for (; nkeys > 0; nkeys--, seq += 2) {
    mrb_hash_set(mrb, hash, seq[0], seq[1]);
  }
  return hash;
}
#else
# define ARG_SPLAT_P(ARGC) ((ARGC) < 0)
# define CI_ARGC_GET(CI) ((CI)->argc)
# define CI_ARG_BLOCK_INDEX(CI) ((CI)->argc >= 0 ? ((CI)->argc + 1 /* self */) : (1 + 1 /* self */))

static bool
match_kwarg_p(mrb_state *mrb, mrb_value obj)
{
  if (!mrb_hash_p(obj)) {
    return false;
  }

  FOREACH_RARRAY(*k, mrbx_expect_ary_ptr_value(mrb, mrb_hash_keys(mrb, obj))) {
    if (!mrb_symbol_p(*k)) {
      return false;
    }
  }

  return true;
}
#endif // MRBX_MRUBY_RELEASE_NO

static void
get_argset(mrb_state *mrb, mrb_int *argc, const mrb_value **argv, mrb_value *argkw, mrb_value *block)
{
  struct mrb_context *c = mrb->c;
  int argc0 = CI_ARGC_GET(c->ci);
  const mrb_value *argv0;
  mrb_value block0;
  const mrb_value *stack = mrbx_vm_top_stacks(c);

  if (ARG_SPLAT_P(argc0)) {
    struct RArray *splat = mrb_ary_ptr(stack[1]);
    argc0 = ARY_LEN(splat);
    argv0 = ARY_PTR(splat);
    block0 = stack[CI_ARG_BLOCK_INDEX(c->ci)];
  } else {
    argv0 = stack + 1;
    block0 = stack[CI_ARG_BLOCK_INDEX(c->ci)];
  }

  if (argkw) {
#if MRBX_MRUBY_RELEASE_NO > 30000
    switch (c->ci->nk) {
    case 0:
      *argkw = mrb_nil_value();
      break;
    case 15:
      *argkw = stack[ARG_COUNT_STACK(c->ci->n) + 1 /* self */];
      break;
    default: // 1..14
      *argkw = pack_kwargs(mrb, c->ci->nk, &stack[ARG_COUNT_STACK(c->ci->n) + 1 /* self */]);
      break;
    }
#else
    if (argc0 > 0 && match_kwarg_p(mrb, argv0[argc0 - 1])) {
      *argkw = argv0[argc0 - 1];
      argc0--;
    } else {
      *argkw = mrb_nil_value();
    }
#endif // MRBX_MRUBY_RELEASE_NO
  }

  if (argc) *argc = argc0;
  if (argv) *argv = argv0;
  if (block) *block = block0;
}

MRB_API void
mrbx_get_argset(mrb_state *mrb, mrb_int *argc, const mrb_value **argv, mrb_value *argkw, mrb_value *block)
{
  get_argset(mrb, argc, argv, argkw, block);
}

MRB_API mrb_int
mrbx_get_argc(mrb_state *mrb)
{
  mrb_int argc;
  get_argset(mrb, &argc, NULL, NULL, NULL);
  return argc;
}

MRB_API const mrb_value *
mrbx_get_argv(mrb_state *mrb)
{
  const mrb_value *argv;
  get_argset(mrb, NULL, &argv, NULL, NULL);
  return argv;
}

MRB_API mrb_value
mrbx_get_arg1(mrb_state *mrb)
{
  mrb_int argc;
  const mrb_value *argv;
  get_argset(mrb, &argc, &argv, NULL, NULL);
  if (argc != 1) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments");
  }
  return argv[0];
}

MRB_API mrb_value
mrbx_get_argblock(mrb_state *mrb)
{
  mrb_value block;
  get_argset(mrb, NULL, NULL, NULL, &block);
  return block;
}

MRB_API mrb_bool
mrbx_given_block_p(mrb_state *mrb)
{
  mrb_value block;
  get_argset(mrb, NULL, NULL, NULL, &block);
  return !mrb_nil_p(block);
}
