#include <mruby-aux/utils.h>
#include <mruby/hash.h>
#include <stdbool.h>
#include <mruby-aux/vmext.h>

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

static void
get_argset(mrb_state *mrb, mrb_int *argc, const mrb_value **argv, mrb_value *argkw, mrb_value *block)
{
  struct mrb_context *c = mrb->c;
  mrb_int argc0 = c->ci->argc;
  const mrb_value *argv0;
  mrb_value block0;

  if (argc0 < 0) {
    struct RArray *splat = mrb_ary_ptr(mrbx_vm_top_stacks(c)[1]);
    argc0 = ARY_LEN(splat);
    argv0 = ARY_PTR(splat);
    block0 = mrbx_vm_top_stacks(c)[2];
  } else {
    argv0 = mrbx_vm_top_stacks(c) + 1;
    block0 = mrbx_vm_top_stacks(c)[1 + argc0];
  }

  if (argkw) {
    if (argc0 > 0 && match_kwarg_p(mrb, argv0[argc0 - 1])) {
      *argkw = argv0[argc0 - 1];
      argc0--;
    } else {
      *argkw = mrb_nil_value();
    }
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
