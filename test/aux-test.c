#include <mruby.h>
#include <mruby-aux.h>
#include <mruby-aux/string.h>
#include <mruby-aux/fakedin.h>
#include <mruby-aux/scanhash.h>
#include <mruby/numeric.h>
#include <mruby/array.h>
#include <mruby/variable.h>

static VALUE
test_str_drop(MRB, VALUE self)
{
    VALUE str;
    mrb_int off, size;
    mrb_get_args(mrb, "Sii", &str, &off, &size);

    return VALUE(mrbx_str_drop(mrb, RSTRING(str), off, size));
}

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

    rest = MRBX_SCANHASH(mrb, target, rest,
            MRBX_SCANHASH_ARG("a", &a, get_default_value(mrb, defaults_ary, 0, req)),
            MRBX_SCANHASH_ARG("b", &b, get_default_value(mrb, defaults_ary, 1, req)),
            MRBX_SCANHASH_ARG("c", &c, get_default_value(mrb, defaults_ary, 2, req)),
            MRBX_SCANHASH_ARG("d", &d, get_default_value(mrb, defaults_ary, 3, req)));

    {
        mrb_value args[] = { rest, a, b, c, d };

        return mrb_yield_argv(mrb, block, sizeof(args) / sizeof(args[0]), args);
    }
}

void mruby_aux_test_fakedin_init(mrb_state *mrb, struct RClass *test);

void
mrb_mruby_aux_gem_test(MRB)
{
    struct RClass *mAuxTest = mrb_define_module(mrb, "AuxTest");

    mrb_define_class_method(mrb, mAuxTest, "str_drop", test_str_drop, MRB_ARGS_REQ(3));

    struct RClass *mAuxScanHashTest = mrb_define_module(mrb, "AuxScanHashTest");
    mrb_define_class_method(mrb, mAuxScanHashTest, "scan_static_keywords", scan_static_keywords, MRB_ARGS_ANY());
    mrb_const_set(mrb, mrb_obj_value(mAuxScanHashTest), id_REQUIREMENT_KEYWORD, mrb_obj_new(mrb, mrb->object_class, 0, NULL));

    mruby_aux_test_fakedin_init(mrb, mAuxTest);
}
