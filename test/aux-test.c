#include <mruby.h>
#include <mruby-aux.h>
#include <mruby-aux/string/drop.h>
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

static const mrb_data_type fakedin_type = {
    "fakedin@mruby-aux",
    mrb_free,
};

static struct mrbx_fakedin *
check_fakedin(MRB, VALUE obj, struct mrbx_fakedin *ptr)
{
    if (!ptr) {
        mrb_raisef(mrb, E_RUNTIME_ERROR,
                   "invalid initialization - %S",
                   obj);
    }

    return ptr;
}

static struct mrbx_fakedin *get_fakedinp(MRB, VALUE obj) { return (struct mrbx_fakedin *)mrb_data_get_ptr(mrb, obj, &fakedin_type); }
static struct mrbx_fakedin *get_fakedin(MRB, VALUE obj) { return check_fakedin(mrb, obj, get_fakedinp(mrb, obj)); }

static VALUE
fakedin_initialize(MRB, VALUE self)
{
    VALUE stream;
    mrb_get_args(mrb, "o", &stream);

    if (DATA_PTR(self) || DATA_TYPE(self)) {
        mrb_raisef(mrb, E_RUNTIME_ERROR,
                   "already initialized - %S",
                   self);
    }

    struct mrbx_fakedin *p = (struct mrbx_fakedin *)mrb_malloc(mrb, sizeof(struct mrbx_fakedin));
    mrbx_fakedin_setup(mrb, self, p, stream);
    mrb_data_init(self, p, &fakedin_type);

    return self;
}

static VALUE
fakedin_read(MRB, VALUE self)
{
    mrb_int size;
    mrb_get_args(mrb, "i", &size);

    const char *buf;
    size = mrbx_fakedin_read(mrb, self, get_fakedin(mrb, self), &buf, size);
    if (size < 0) {
        return Qnil;
    }

    return mrb_str_new(mrb, buf, size);
}

static VALUE
fakedin_eof(MRB, VALUE self)
{
    mrb_get_args(mrb, "");

    return mrb_bool_value(mrbx_fakedin_eof(get_fakedin(mrb, self)));
}

static VALUE
fakedin_tell(MRB, VALUE self)
{
    mrb_get_args(mrb, "");

    return mrb_fixnum_value(mrbx_fakedin_total_in(get_fakedin(mrb, self)));
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

void
mrb_mruby_aux_gem_test(MRB)
{
    struct RClass *mAuxTest = mrb_define_module(mrb, "AuxTest");

    mrb_define_class_method(mrb, mAuxTest, "str_drop", test_str_drop, MRB_ARGS_REQ(3));

    struct RClass *cFakedIn = mrb_define_class_under(mrb, mAuxTest, "FakedIn", mrb->object_class);
    MRB_SET_INSTANCE_TT(cFakedIn, MRB_TT_DATA);
    mrb_define_method(mrb, cFakedIn, "initialize", fakedin_initialize, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, cFakedIn, "read", fakedin_read, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, cFakedIn, "eof?", fakedin_eof, MRB_ARGS_NONE());
    mrb_define_method(mrb, cFakedIn, "tell", fakedin_tell, MRB_ARGS_NONE());

    struct RClass *mAuxScanHashTest = mrb_define_module(mrb, "AuxScanHashTest");
    mrb_define_class_method(mrb, mAuxScanHashTest, "scan_static_keywords", scan_static_keywords, MRB_ARGS_ANY());
    mrb_const_set(mrb, mrb_obj_value(mAuxScanHashTest), id_REQUIREMENT_KEYWORD, mrb_obj_new(mrb, mrb->object_class, 0, NULL));
}
