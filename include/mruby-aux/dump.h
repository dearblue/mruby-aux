#ifndef MRUBY_AUX_DUMP_H
#define MRUBY_AUX_DUMP_H 1

#include "compat/dump.h"

MRB_API mrb_bool mrbx_taste_irep_buf(mrb_state *mrb, const void *buf, size_t bufsize, mrb_value name, mrb_bool raise_if_error);
MRB_API void mrbx_err_irep_buf(mrb_state *mrb, const void *buf, size_t bufsize, mrb_value name);

#endif /* MRUBY_AUX_DUMP_H */
