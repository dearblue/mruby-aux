#include <mruby-aux/array.h>

static mrb_value
test_tuple_3(mrb_state *mrb, mrb_value self)
{
    return MRBX_TUPLE(mrb_fixnum_value(1), mrb_fixnum_value(2), mrb_fixnum_value(3));
}

static mrb_value
test_tuple_5(mrb_state *mrb, mrb_value self)
{
    return MRBX_TUPLE(mrb_fixnum_value(1),
                      mrb_fixnum_value(2),
                      mrb_fixnum_value(3),
                      mrb_fixnum_value(4),
                      mrb_fixnum_value(5));
}

static mrb_value
test_ary_ptr(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "A!", &obj);

    struct RArray *ary = mrbx_ary_ptr(mrb, obj);

    if (ary) {
        return mrb_obj_value(ary);
    } else {
        return mrb_nil_value();
    }
}

static mrb_value
test_ary_ptr_double(mrb_state *mrb, mrb_value self)
{
    mrb_value obj;
    mrb_get_args(mrb, "A!", &obj);

    struct RArray *ary = mrbx_ary_ptr(mrb, mrbx_ary_ptr(mrb, obj));

    if (ary) {
        return mrb_obj_value(ary);
    } else {
        return mrb_nil_value();
    }
}

void
mruby_aux_test_array_init(mrb_state *mrb, struct RClass *test)
{
    mrb_define_class_method(mrb, test, "tuple_3",         test_tuple_3,        MRB_ARGS_ANY());
    mrb_define_class_method(mrb, test, "tuple_5",         test_tuple_5,        MRB_ARGS_ANY());
    mrb_define_class_method(mrb, test, "ary_ptr",         test_ary_ptr,        MRB_ARGS_ANY());
    mrb_define_class_method(mrb, test, "ary_ptr_double ", test_ary_ptr_double, MRB_ARGS_ANY());
}
