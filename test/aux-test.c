#include <mruby.h>
#include <mruby-aux.h>
#include <mruby-aux/string.h>
#include <mruby-aux/fakedin.h>
#include <mruby-aux/scanhash.h>
#include <mruby/numeric.h>
#include <mruby/array.h>
#include <mruby/variable.h>

void mruby_aux_test_array_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_fakedin_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_scanhash_init(mrb_state *mrb, struct RClass *test);
void mruby_aux_test_string_init(mrb_state *mrb, struct RClass *test);

void
mrb_mruby_aux_gem_test(MRB)
{
    struct RClass *mAuxTest = mrb_define_module(mrb, "AuxTest");

    mruby_aux_test_array_init(mrb, mAuxTest);
    mruby_aux_test_fakedin_init(mrb, mAuxTest);
    mruby_aux_test_scanhash_init(mrb, mAuxTest);
    mruby_aux_test_string_init(mrb, mAuxTest);
}
