#include <mruby.h>

#if MRUBY_RELEASE_NO < 20001

#include <mruby-aux/compat/hash.h>

MRB_API void
mrb_hash_foreach(mrb_state *mrb, struct RHash *hash, mrb_hash_foreach_func *func, void *userdata)
{
  mrb_value keys = mrb_hash_keys(mrb, mrb_obj_value(hash));
  mrb_value values = mrb_hash_values(mrb, mrb_obj_value(hash));
  const mrb_value *k = RARRAY_PTR(keys);
  const mrb_value *const kk = k + RARRAY_LEN(keys);
  const mrb_value *v = RARRAY_PTR(values);
  int arena = mrb_gc_arena_save(mrb);

  for (; k < kk; k++, v++) {
    if (func(mrb, *k, *v, userdata) != 0) { return; }
    mrb_gc_arena_restore(mrb, arena);
  }
}

#endif
