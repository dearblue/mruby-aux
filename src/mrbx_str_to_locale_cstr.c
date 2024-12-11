#include <mruby-aux/string.h>
#include "string.h"

MRB_API char *
mrbx_str_to_locale_cstr(mrb_state *mrb, mrb_value str)
{
#ifndef _WIN32
  return mrb_str_to_cstr(mrb, str);
#else
  mrb_ensure_string_type(mrb, str);
  mrbx_str_check_null_byte(mrb, mrb_str_ptr(str));

  struct RString *s = mrbx_win32_str_new_cstr_by_cp(mrb, RSTRING_PTR(str), RSTRING_LEN(str), CP_UTF8, CP_ACP);

  return RSTR_PTR(s);
#endif
}
