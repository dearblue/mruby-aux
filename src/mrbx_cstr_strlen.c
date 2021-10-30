#include <mruby-aux/string.h>

MRB_API size_t
mrbx_cstrlen(const char *str, const char *end)
{
#ifdef MRB_UTF8_STRING
  return mrbx_utf8_cstrlen(str, end);
#else
  if (str == NULL || str >= end || (end - str) <= 0) {
    return 0;
  } else {
    return (size_t)(end - str);
  }
#endif
}

MRB_API size_t
mrbx_utf8_cstrlen(const char *str, const char *end)
{
  ptrdiff_t n = end - str;
  if (n < 1 || str == NULL || str > end) {
    return 0;
  }

  size_t len = 0;
  while (n > 0) {
    size_t c = mrbx_utf8_charlen(str, end);
    str += c;
    n -= c;
    len++;
  }
  return len;
}
