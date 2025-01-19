#include <mruby-aux/string.h>
#include "string.h"

MRB_API const char *
mrbx_string_locale_cstr(mrb_state *mrb, mrb_value str)
{
#ifndef _WIN32
  return mrb_string_cstr(mrb, str);
#else
  mrb_ensure_string_type(mrb, str);

  if (RSTRING_LEN(str) == 0) {
    return "";
  }

  mrbx_str_check_null_byte(mrb, mrb_str_ptr(str));

  struct RString *s = mrbx_win32_str_new_cstr_by_cp(mrb, RSTRING_PTR(str), RSTRING_LEN(str), CP_UTF8, CP_ACP);

  return RSTR_PTR(s);
#endif
}
