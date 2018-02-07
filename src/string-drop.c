#include <mruby-aux/string.h>
#include <string.h>

struct RString *
mrbx_str_drop(mrb_state *mrb, struct RString *str, mrb_int off, mrb_int size)
{
    mrb_int len = RSTR_LEN(str);

    mrb_str_modify(mrb, str);

    if (off > len) {
        off = 0;
    } else {
        if (off < 0) {
            off += len;
            if (off < 0) {
                mrb_raise(mrb, E_INDEX_ERROR, "wrong string index");
            }
        }

        if (size < 0 || size > (len - off)) {
            ;
        } else {
            char *p = RSTR_PTR(str);
            memcpy(p + off, p + off + size, len - (off + size));
            off += size;
        }
    }

    RSTR_SET_LEN(str, off);

    return str;
}
