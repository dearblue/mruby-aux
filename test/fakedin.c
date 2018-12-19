#include <mruby-aux/fakedin.h>
#include <mruby/class.h>
#include <mruby/data.h>

static const mrb_data_type fakedin_type = {
    "fakedin@mruby-aux",
    mrb_free,
};

static struct mrbx_fakedin *
check_fakedin(mrb_state *mrb, mrb_value obj, struct mrbx_fakedin *ptr)
{
    if (!ptr) {
        mrb_raisef(mrb, E_RUNTIME_ERROR,
                   "invalid initialization - %S",
                   obj);
    }

    return ptr;
}

static struct mrbx_fakedin *get_fakedinp(mrb_state *mrb, mrb_value obj) { return (struct mrbx_fakedin *)mrb_data_get_ptr(mrb, obj, &fakedin_type); }
static struct mrbx_fakedin *get_fakedin(mrb_state *mrb, mrb_value obj) { return check_fakedin(mrb, obj, get_fakedinp(mrb, obj)); }

static mrb_value
fakedin_initialize(mrb_state *mrb, mrb_value self)
{
    mrb_value stream;
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

static mrb_value
fakedin_read(mrb_state *mrb, mrb_value self)
{
    mrb_int size;
    mrb_get_args(mrb, "i", &size);

    const char *buf;
    size = mrbx_fakedin_read(mrb, self, get_fakedin(mrb, self), &buf, size);
    if (size < 0) {
        return mrb_nil_value();
    }

    return mrb_str_new(mrb, buf, size);
}

static mrb_value
fakedin_eof(mrb_state *mrb, mrb_value self)
{
    mrb_get_args(mrb, "");

    return mrb_bool_value(mrbx_fakedin_eof(get_fakedin(mrb, self)));
}

static mrb_value
fakedin_tell(mrb_state *mrb, mrb_value self)
{
    mrb_get_args(mrb, "");

    return mrb_fixnum_value(mrbx_fakedin_total_in(get_fakedin(mrb, self)));
}

void
mruby_aux_test_fakedin_init(mrb_state *mrb, struct RClass *test)
{
    struct RClass *cFakedIn = mrb_define_class_under(mrb, test, "FakedIn", mrb->object_class);
    MRB_SET_INSTANCE_TT(cFakedIn, MRB_TT_DATA);
    mrb_define_method(mrb, cFakedIn, "initialize", fakedin_initialize, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, cFakedIn, "read", fakedin_read, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, cFakedIn, "eof?", fakedin_eof, MRB_ARGS_NONE());
    mrb_define_method(mrb, cFakedIn, "tell", fakedin_tell, MRB_ARGS_NONE());
}
