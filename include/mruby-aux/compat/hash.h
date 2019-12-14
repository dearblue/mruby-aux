#ifndef MRUBY_AUX_COMAPT_HASH_H
#define MRUBY_AUX_COMAPT_HASH_H 1

#include "../hash.h"

/* mrb_hash_empty_p() は mruby-1.4.1 までは mrb_value を返す */
#include <mruby/object.h>
#if !defined(MRB_FL_OBJ_IS_FROZEN)
# define mrb_hash_empty_p(MRB, HASH)  mrb_bool(mrb_hash_empty_p(MRB, HASH))
#endif

#if MRUBY_RELEASE_NO <= 10200
MRB_API mrb_value mrb_hash_values(mrb_state *mrb, mrb_value hash);
#endif

#if MRUBY_RELEASE_NO < 20001
typedef int mrb_hash_foreach_func(mrb_state *mrb, mrb_value key, mrb_value value, void *userdata);
MRB_API void mrb_hash_foreach(mrb_state *mrb, struct RHash *hash, mrb_hash_foreach_func *func, void *userdata);
#endif

#endif /* MRUBY_AUX_COMAPT_HASH_H */
