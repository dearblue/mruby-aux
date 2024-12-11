#include <mruby-aux/utils.h>
#include <mruby/string.h>
#include "string.h"

#ifdef _WIN32

struct RString *
mrbx_win32_str_new_cstr_by_cp(mrb_state *mrb, const char *src, intptr_t srclen, UINT srccp, UINT destcp)
{
# if INTPTR_MAX > INT_MAX
  if (srclen > INT_MAX) {
    mrb_raise(mrb, E_RUNTIME_ERROR,
              "failed to encode string too big (limited to less than " MRB_STRINGIZE(INT_MAX) " bytes)");
  }
# endif

  if (src == NULL || srclen == 0 || src[0] == '\0') {
    return mrb_str_ptr(mrb_str_new(mrb, NULL, 0));
  }
  else if (srclen < 0) {
    srclen = strlen(src);
  }

  int wcslen = MultiByteToWideChar(srccp, 0, src, srclen, NULL, 0);
  if (wcslen <= 0) {
  failed:
    mrb_raise(mrb, E_RUNTIME_ERROR, "failed to encode string");
  }

# define WCS_ONSTACK_MAX 128 // 256 bytes
  wchar_t onstack[WCS_ONSTACK_MAX];
  int ai = mrb_gc_arena_save(mrb);
  wchar_t *wcsp = wcslen <= WCS_ONSTACK_MAX ? onstack : (wchar_t *)mrbx_alloca(mrb, wcslen * sizeof(wchar_t));
  wcslen = MultiByteToWideChar(srccp, 0, src, srclen, wcsp, wcslen);
  if (wcslen <= 0) {
    goto failed;
  }

  int mbslen = WideCharToMultiByte(destcp, 0, wcsp, wcslen, NULL, 0, NULL, NULL);
  if (mbslen <= 0) {
    goto failed;
  }

  struct RString *s = mrb_str_ptr(mrb_str_new(mrb, NULL, mbslen));
  mbslen = WideCharToMultiByte(destcp, 0, wcsp, wcslen, RSTR_PTR(s), RSTR_LEN(s), NULL, NULL);
  if (mbslen <= 0) {
    goto failed;
  }

  mrb_gc_arena_restore(mrb, ai);
  mrb_gc_protect(mrb, mrb_obj_value(s));

  return s;
}

#endif // _WIN32
