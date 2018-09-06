#ifndef MRUBY_AUX_STRING_DROP_H_
#define MRUBY_AUX_STRING_DROP_H_ 1

#include "../string.h"

MRB_BEGIN_DECL

struct RString *mrbx_str_drop(mrb_state *mrb, struct RString *str, mrb_int off, mrb_int size);

MRB_END_DECL

#endif /* MRUBY_AUX_STRING_DROP_H_ */
