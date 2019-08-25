#ifndef MRUBY_AUX_FAKEDIN_H_
#define MRUBY_AUX_FAKEDIN_H_ 1

#include <mruby.h>
#include <stdint.h>

MRB_BEGIN_DECL

MRB_API mrb_value mrbx_fakedin_new(mrb_state *mrb, mrb_value stream);
MRB_API mrb_int mrbx_fakedin_read(mrb_state *mrb, mrb_value fakedin, const char **buf, mrb_int size);
MRB_API mrb_bool mrbx_fakedin_eof(mrb_state *mrb, mrb_value fakedin);
MRB_API int64_t mrbx_fakedin_total_in(mrb_state *mrb, mrb_value fakedin);

MRB_END_DECL

#endif /* MRUBY_AUX_FAKEDIN_H_ */
