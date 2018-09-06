#ifndef MRUBY_AUX_STRING_HEXDIGEST_H_
#define MRUBY_AUX_STRING_HEXDIGEST_H_ 1

#include "../string.h"

MRB_BEGIN_DECL

mrb_value mrbx_str_new_as_hexdigest(mrb_state *mrb, uint64_t n, int bytesize);

MRB_END_DECL

#endif /* MRUBY_AUX_STRING_HEXDIGEST_H_ */
