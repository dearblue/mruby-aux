#include <mruby-aux/string.h>
#include <mruby-aux/utils.h>

MRB_API size_t
mrbx_charlen_unsafe(const char *str, const char *end)
{
#ifdef MRB_UTF8_STRING
  return mrbx_utf8_charlen_unsafe(str, end);
#else
  return 1;
#endif
}

MRB_API size_t
mrbx_charlen(const char *str, const char *end)
{
  if (mrbx_out_of_pointer_p(str, end)) {
    return 0;
  } else {
    return mrbx_charlen_unsafe(str, end);
  }
}

MRB_API size_t
mrbx_utf8_charlen_unsafe(const char *str, const char *end)
{
  ptrdiff_t l = end - str;
  const unsigned char *u = (const unsigned char *)str;
  switch (u[0] >> 3) {
  default: // 0x00 .. 0x7f, 0x80 .. 0xbf, 0xf8 .. 0xff
    break;

  // U+0080..U+07ff
  case 0x18: // 0xc0
  case 0x19: // 0xc8
  case 0x1a: // 0xd0
  case 0x1b: // 0xd8 .. 0xdf
    if (l >= 2 && (u[1] & 0xc0) == 0x80 &&
        u[0] >= 0xc2) {
      return 2;
    }
    break;

  // U+0800..U+ffff
  case 0x1c: // 0xe0
  case 0x1d: // 0xe8 .. 0xef
    if (l >= 3 && (((u[1] ^ 0x80) | (u[2] ^ 0x80)) & 0xc0) == 0x00 &&
        (u[0] >= 0xe1 || u[1] >= 0xa0)) {
      return 3;
    }
    break;

  // U+0x010000..U+10ffff
  case 0x1e: // 0xf0 .. 0xf7
    if (l >= 4 && (((u[1] ^ 0x80) | (u[2] ^ 0x80) | (u[3] ^ 0x80)) & 0xc0) == 0x00) {
      switch (u[0]) {
      case 0xf0:
        if (u[1] >= 0x90) {
          return 4;
        }
        break;
      case 0xf1:
      case 0xf2:
      case 0xf3:
        return 4;
      case 0xf4:
        if (u[1] <= 0x8f) {
          return 4;
        }
        break;
      }
    }
    break;
  }

  return 1;
}

MRB_API size_t
mrbx_utf8_charlen(const char *str, const char *end)
{
  if (mrbx_out_of_pointer_p(str, end)) {
    return 0;
  } else {
    return mrbx_utf8_charlen_unsafe(str, end);
  }
}
