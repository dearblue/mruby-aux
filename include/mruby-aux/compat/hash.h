#ifndef MRUBY_AUX_COMAPT_HASH_H
#define MRUBY_AUX_COMAPT_HASH_H 1

#include "../hash.h"

/* mrb_hash_empty_p() は mruby-1.4.1 までは mrb_value を返す */
#include <mruby/object.h>
#if !defined(MRB_FL_OBJ_IS_FROZEN)
# define mrb_hash_empty_p(MRB, HASH)  mrb_bool(mrb_hash_empty_p(MRB, HASH))
#endif

#endif /* MRUBY_AUX_COMAPT_HASH_H */
