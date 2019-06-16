#include <mruby-aux/compat/mruby.h>
#include <mruby/string.h>

#if MRUBY_RELEASE_NO < 20000
MRB_API mrb_value
mrb_to_str(mrb_state *mrb, mrb_value obj)
{
    return mrb_str_to_str(mrb, obj);
}
#endif
