#ifndef MRUBY_AUX_STRING_HEXDIGEST_H_
#define MRUBY_AUX_STRING_HEXDIGEST_H_ 1

#include "../string.h"

MRBX_UNUSED static mrb_value
mrbx_str_new_as_hexdigest(mrb_state *mrb, uint64_t n, int bytesize)
{
    if (bytesize < 1) {
        bytesize = sizeof(size_t);
    } else if (bytesize > sizeof(n)) {
        bytesize = sizeof(n);
    }

    int off = bytesize * 8;
    char str[bytesize * 2];
    char *p = str;

    for (; off > 0; off -= 4, p ++) {
        uint8_t ch = (n >> (off - 4)) & 0x0f;
        if (ch < 10) {
            *p = ch + '0';
        } else {
            *p = ch + ('a' - 10);
        }
    }

    return mrb_str_new(mrb, str, bytesize * 2);
}

#endif /* MRUBY_AUX_STRING_HEXDIGEST_H_ */
