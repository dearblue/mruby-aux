#include <mruby-aux/string.h>
#include "string.h"

MRB_API mrb_value
mrbx_str_new_locale_cstr(mrb_state *mrb, const char *p)
{
#ifndef _WIN32
  return mrb_str_new_cstr(mrb, p);
#else
  return mrb_obj_value(mrbx_win32_str_new_cstr_by_cp(mrb, p, -1, CP_ACP, CP_UTF8));
#endif
}
