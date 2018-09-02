#include <mruby-aux/string/hexdigest.h>

mrb_value
mrbx_str_new_as_hexdigest(mrb_state *mrb, uint64_t n, int bytesize)
{
    static const char hexmap[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };

    if (bytesize < 1) {
        bytesize = sizeof(size_t);
    } else if (bytesize > sizeof(n)) {
        bytesize = sizeof(n);
    }

    int off = bytesize * 8;
    mrb_value str = mrb_str_new(mrb, NULL, bytesize * 2);
    char *p = RSTRING_PTR(str);

    for (; off > 0; off -= 4, p ++) {
        *p = hexmap[(n >> (off - 4)) & 0x0f];
    }

    return str;
}
