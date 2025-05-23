#include <mruby-aux/scanhash.h>
#include <mruby/variable.h>

#define id_REQUIREMENT_KEYWORD mrb_intern_lit(mrb, "REQUIREMENT_KEYWORD")

static mrb_value
get_default_value(mrb_state *mrb, struct RArray *ary, mrb_int index, mrb_value requirement)
{
  if (index < 0 || ARY_LEN(ary) < index - 1) {
    return mrb_nil_value();
  }

  mrb_value v = ARY_PTR(ary)[index];

  if (mrb_obj_eq(mrb, v, requirement)) {
    return mrb_undef_value();
  } else {
    return v;
  }
}

static mrb_value
scan_static_keywords(mrb_state *mrb, mrb_value mAuxScanHashTest)
{
  mrb_value a, b, c, d;
  mrb_value target, rest, defaults, block;
  mrb_get_args(mrb, "ooA&", &target, &rest, &defaults, &block);

  struct RArray *defaults_ary = RARRAY(defaults);
  mrb_value req = mrb_const_get(mrb, mAuxScanHashTest, id_REQUIREMENT_KEYWORD);

  if (mrb_hash_p(rest)) {
    MRBX_SCANHASH(mrb, target, rest,
                  MRBX_SCANHASH_ARGS("a", &a, get_default_value(mrb, defaults_ary, 0, req)),
                  MRBX_SCANHASH_ARGS("b", &b, get_default_value(mrb, defaults_ary, 1, req)),
                  MRBX_SCANHASH_ARGS("c", &c, get_default_value(mrb, defaults_ary, 2, req)),
                  MRBX_SCANHASH_ARGS("d", &d, get_default_value(mrb, defaults_ary, 3, req)));
  } else {
    struct mrbx_scanhash_arg args[] = {
      MRBX_SCANHASH_ARGS("a", &a, get_default_value(mrb, defaults_ary, 0, req)),
      MRBX_SCANHASH_ARGS("b", &b, get_default_value(mrb, defaults_ary, 1, req)),
      MRBX_SCANHASH_ARGS("c", &c, get_default_value(mrb, defaults_ary, 2, req)),
      MRBX_SCANHASH_ARGS("d", &d, get_default_value(mrb, defaults_ary, 3, req))
    };

    rest = mrbx_scanhash(mrb, target, rest, ELEMENTOF(args), args);
  }

  {
    mrb_value args[] = { rest, a, b, c, d };

    return mrb_yield_argv(mrb, block, sizeof(args) / sizeof(args[0]), args);
  }
}

void
mruby_aux_test_scanhash_init(mrb_state *mrb, struct RClass *test)
{
  mrb_define_class_method(mrb, test, "scan_static_keywords", scan_static_keywords, MRB_ARGS_ANY());
  mrb_const_set(mrb, mrb_obj_value(test), id_REQUIREMENT_KEYWORD, mrb_obj_new(mrb, mrb->object_class, 0, NULL));
}
