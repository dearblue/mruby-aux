#include <mruby-aux/string.h>

MRB_API mrb_value
mrbx_str_new_static(mrb_state *mrb, const char *str, intptr_t len)
{
  if (str && len != 0) {
    if (len < 0) {
      len = strlen(str);
    }
  } else {
    str = NULL;
    len = 0;
  }

  return mrb_str_new_static(mrb, str, len);
}
