#ifndef MRUBY_AUX_HASH_H
#define MRUBY_AUX_HASH_H 1

#include "common.h"
#include <mruby.h>
#include <mruby/hash.h>

MRBX_INLINE struct RHash *
mrbx_hash_new(mrb_state *mrb)
{
    return mrb_hash_ptr(mrb_hash_new(mrb));
}

MRBX_INLINE struct RHash *
mrbx_hash_new_capa(mrb_state *mrb, mrb_int size)
{
    return mrb_hash_ptr(mrb_hash_new_capa(mrb, size));
}

MRBX_INLINE struct RHash *
mrbx_hash_ptr(mrb_state *mrb, mrb_value hash)
{
    if (mrb_nil_p(hash)) { return NULL; }
    mrb_check_type(mrb, hash, MRB_TT_HASH);
    return mrb_hash_ptr(hash);
}

MRBX_INLINE struct RHash *
mrbx_by_hash_ptr(mrb_state *mrb, struct RHash *hash)
{
    return hash;
}

#ifdef __cplusplus

MRBX_INLINE struct RHash *mrbx_hash_ptr(mrb_state *mrb, struct RHash *p) { return mrbx_by_hash_ptr(mrb, p); }

#else

# define mrbx_hash_ptr(MRB, V)                                              \
        _Generic((V),                                                       \
                 mrb_value:             mrbx_hash_ptr,                      \
                 struct RHash *:        mrbx_by_hash_ptr)                   \
            (MRB, V)                                                        \

#endif

#endif /* MRUBY_AUX_HASH_H */
