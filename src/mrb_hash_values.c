#include <mruby-aux/compat/hash.h>

#if MRUBY_RELEASE_NO <= 10200
MRB_API mrb_value
mrb_hash_values(mrb_state *mrb, mrb_value hash)
{
  return mrb_funcall_argv(mrb, hash, mrb_intern_lit(mrb, "values"), 0, NULL);
}
#endif
