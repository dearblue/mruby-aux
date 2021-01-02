#include <mruby-aux/string.h>

static const char hexmap[16] = {
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};

static const char HEXMAP[16] = {
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
};

static int
correct_bytesize(int bytesize)
{
  if (bytesize < 1) {
    return sizeof(size_t);
  } else if (bytesize > sizeof(uint64_t)) {
    return sizeof(uint64_t);
  } else {
    return bytesize;
  }
}

static void
cstr_hexdigest_common(char str[], uint64_t n, int bytesize, const char hexmap[])
{
  int width = bytesize * 2;
  char *p = str + width - 1;

  for (int i = width; i > 0; i--, p--) {
    *p = hexmap[n & 0x0f];
    n >>= 4;
  }
}

MRB_API char *
mrbx_cstr_hexdigest(char buf[], uint64_t n, int bytesize)
{
  bytesize = correct_bytesize(bytesize);
  cstr_hexdigest_common(buf, n, bytesize, hexmap);
  return buf;
}

MRB_API char *
mrbx_cstr_HEXDIGEST(char buf[], uint64_t n, int bytesize)
{
  bytesize = correct_bytesize(bytesize);
  cstr_hexdigest_common(buf, n, bytesize, HEXMAP);
  return buf;
}

static struct RString *
str_hexdigest_common(mrb_state *mrb, uint64_t n, int bytesize, const char hexmap[])
{
  bytesize = correct_bytesize(bytesize);
  struct RString *str = RSTRING(mrb_str_new(mrb, NULL, bytesize * 2));
  cstr_hexdigest_common(RSTR_PTR(str), n, bytesize, hexmap);
  return str;
}

MRB_API struct RString *
mrbx_str_new_as_hexdigest(mrb_state *mrb, uint64_t n, int bytesize)
{
  return str_hexdigest_common(mrb, n, bytesize, hexmap);
}

MRB_API struct RString *
mrbx_str_new_as_HEXDIGEST(mrb_state *mrb, uint64_t n, int bytesize)
{
  return str_hexdigest_common(mrb, n, bytesize, HEXMAP);
}
