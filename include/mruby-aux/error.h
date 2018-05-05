#ifndef MRUBY_AUX_ERROR_H
#define MRUBY_AUX_ERROR_H 1

#include <mruby.h>
#include "common.h"

MRB_BEGIN_DECL

mrb_value
mrbx_protect_exceptions(
        mrb_state *mrb,
        mrb_func_t body, mrb_value b_data,
        mrb_func_t rescue, mrb_value r_data,
        mrb_func_t ensure, mrb_value e_data,
        mrb_int len, struct RClass *classes[]);

MRB_END_DECL

#endif /* MRUBY_AUX_ERROR_H */
