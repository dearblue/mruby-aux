#include <mruby-aux/string.h>

struct RString *
mrbx_str_buf_growup(mrb_state *mrb, struct RString *str, size_t maxsize, mrb_bool *is_partial, mrbx_str_buf_growup_f *func, void *user)
{
#ifdef MRBX_SMALL_TARGET
    enum { first_growup = 256, max_growup = 1 * 1024, };
#else
    enum { first_growup = 4 * 1024, max_growup = 1 * 1024 * 1024, };
#endif

    if ((intptr_t)maxsize < 0) {
        maxsize = MRBX_STR_MAX;
    } else if (maxsize > MRBX_STR_MAX) {
        mrb_raise(mrb, E_RUNTIME_ERROR,
                  "maybe string capacity limitation");
    }

    str = mrbx_str_force_recycle(mrb, str, first_growup);

    size_t off = RSTR_LEN(str);
    mrb_int grow = (first_growup < maxsize ? first_growup : maxsize);

    for (;;) {
        mrbx_str_reserve(mrb, str, off + grow);

        size_t step = grow;
        intptr_t s = func(mrb, RSTR_PTR(str) + off, &step, user);
        off += step;
        if (s >= 0) {
            if ((off + s) < s || (off + s) > maxsize) {
                RSTR_SET_LEN(str, off);

                mrb_raise(mrb, E_RUNTIME_ERROR,
                          "requested string capacity limitation");
            }
            grow = s;
        } else if (s == MRBX_NEXT) {
            grow *= 2;
            grow = (grow < max_growup && grow > 0 ? grow : max_growup);

            if ((off + grow) < grow || (off + grow) > maxsize) {
                grow = maxsize - off;
                if (grow == 0) {
                    if (is_partial && *is_partial) {
                        *is_partial = TRUE;
                        break;
                    } else {
                        RSTR_SET_LEN(str, off);

                        mrb_raise(mrb, E_RUNTIME_ERROR,
                                  "reached string capacity limitation");
                    }
                }
            }
        } else if (s == MRBX_STOP) {
            if (is_partial) { *is_partial = FALSE; }
            break;
        } else {
            mrb_gc_arena_restore(mrb, 0);
            mrb_bug(mrb, "invalid expand operation for %S",
                    mrb_str_new_cstr(mrb, __func__));
        }
    }

    RSTR_SET_LEN(str, off);

    return str;
}
