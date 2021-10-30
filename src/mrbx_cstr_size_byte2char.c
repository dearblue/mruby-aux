#include <mruby-aux/string.h>

MRB_API intptr_t
mrbx_cstr_size_byte2char(const char *str, const char *end, size_t nbytes)
{
#ifdef MRB_UTF8_STRING
  return mrbx_utf8_size_byte2char(str, end, nbytes);
#else
  if ((ptrdiff_t)nbytes < 1 || str == NULL || str > end || (end - str) < (ptrdiff_t)nbytes) {
    return 0;
  } else {
    return (intptr_t)nbytes;
  }
#endif
}

MRB_API intptr_t
mrbx_utf8_size_byte2char(const char *str, const char *end, size_t nbytes)
{
  if ((ptrdiff_t)nbytes < 1 || str == NULL || str > end || (end - str) < (ptrdiff_t)nbytes) {
    return 0;
  }

  intptr_t nchars = 0;
  while (str < end) {
    size_t n = mrbx_utf8_charlen(str, end);
    nchars++;
    nbytes -= n;
    str += n;
    if (nbytes == 0) {
      break;
    } else if (nbytes < n) {
      // UTF-8 文字境界に一致しない
      break;
    }
  }
  return nchars;
}
