#ifndef MRUBY_AUX_STRING_DROP_H_
#define MRUBY_AUX_STRING_DROP_H_ 1

#include "../string.h"

struct RString *mrbx_str_drop(mrb_state *mrb, struct RString *str, mrb_int off, mrb_int size);

#endif /* MRUBY_AUX_STRING_DROP_H_ */
