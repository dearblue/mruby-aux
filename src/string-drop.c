#include <mruby-aux/string.h>
#include <string.h>

struct RString *
mrbx_str_drop(mrb_state *mrb, struct RString *str, mrb_int off, mrb_int size)
{
    mrb_int len = RSTR_LEN(str);

    mrb_str_modify(mrb, str);

    if (off < 0) {
        mrb_int off0 = off;

        off += len;

        if (off < 0) {
            mrb_raisef(mrb, E_INDEX_ERROR,
                       "out of string (expect %S..%S, but given %S)",
                       mrb_fixnum_value(-len),
                       mrb_fixnum_value(len - 1),
                       mrb_fixnum_value(off0));
        }
    }

    if (off > len) {
        off = len;
    } else {
        if (size < 1) {
            off = len;
        } else if (size > (len - off)) {
            ;
        } else {
            char *p = RSTR_PTR(str) + off;
            memmove(p, p + size, len - (off + size));
            off = len - size;
        }
    }

    RSTR_SET_LEN(str, off);

    return str;
}
