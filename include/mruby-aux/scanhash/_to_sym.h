/*
 * This code is under public domain (CC0)
 * <https://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * To the extent possible under law, dearblue has waived all copyright
 * and related or neighboring rights to this work.
 *
 *     dearblue <dearblue@users.noreply.github.com>
 */

/*
 * NOTE: MRBX_SCANHASH_TO_SYMBOL() の定義
 */

#ifndef MRUBY_AUX_SCANHASH_TO_SYM_H_
#define MRUBY_AUX_SCANHASH_TO_SYM_H_ 1

#if __cplusplus
#   if __cplusplus < 201703L
#       define MRBX_SCANHASH_UNUSED inline
#   else
#       define MRBX_SCANHASH_UNUSED [[maybe_unused]]
#   endif
#elif defined(__GNUC__) || defined (__clang__)
#   define MRBX_SCANHASH_UNUSED __attribute__((unused))
#else
#   define MRBX_SCANHASH_UNUSED
#endif

#ifdef __cplusplus

template <typename T>
MRBX_SCANHASH_UNUSED static mrb_sym
MRBX_SCANHASH_TO_SYMBOL(mrb_state *mrb, T str)
{
    static_assert(sizeof(T) < 0, "wrong type");
}

MRBX_SCANHASH_UNUSED static mrb_sym
MRBX_SCANHASH_TO_SYMBOL(mrb_state *mrb, mrb_sym str)
{
    return str;
}

MRBX_SCANHASH_UNUSED static mrb_sym
MRBX_SCANHASH_TO_SYMBOL(mrb_state *mrb, char *str)
{
    return mrb_intern_cstr(mrb, str);
}

MRBX_SCANHASH_UNUSED static mrb_sym
MRBX_SCANHASH_TO_SYMBOL(mrb_state *mrb, const char *str)
{
    return mrb_intern_cstr(mrb, str);
}

#else

MRBX_SCANHASH_UNUSED static mrb_sym
MRBX_SCANHASH_TO_SYMBOL_symbol(mrb_state *mrb, mrb_sym sym)
{
    return sym;
}

#   define MRBX_SCANHASH_TO_SYMBOL(mrb, str)            \
    _Generic((str),                                     \
             mrb_sym: MRBX_SCANHASH_TO_SYMBOL_symbol,   \
             char *: mrb_intern_cstr,                   \
             const char *: mrb_intern_cstr)             \
        ((mrb), (str))                                  \

#endif

#endif /* MRUBY_AUX_SCANHASH_TO_SYM_H_ */
