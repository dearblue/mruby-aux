#ifndef MRUBY_AUX_COMMON_H
#define MRUBY_AUX_COMMON_H 1

#include <mruby.h>

#ifndef MRBX_INLINE
# define MRBX_INLINE MRB_INLINE
#endif

#ifndef MRBX_FORCE_INLINE
# ifdef MRB_FORCE_INLINE
#  define MRBX_FORCE_INLINE MRB_FORCE_INLINE
# elif defined(__GNUC__) || defined (__clang__)
#  define MRBX_FORCE_INLINE __attribute__((always_inline))
# elif defined _MSC_VER
#  define MRBX_FORCE_INLINE __forceinline
# else
#  define MRBX_FORCE_INLINE MRBX_INLINE
# endif
#endif

#ifndef MRBX_NOINLINE
# ifdef MRB_NOINLINE
#  define MRBX_NOINLINE MRB_NOINLINE
# elif defined(__GNUC__) || defined (__clang__)
#  define MRBX_NOINLINE __attribute__((noinline))
# elif defined _MSC_VER
#  define MRBX_NOINLINE __declspec(noinline)
# else
#  define MRBX_NOINLINE static
# endif
#endif

#ifndef MRBX_WEAK
# if defined(__GNUC__) || defined (__clang__)
#  define MRBX_WEAK __attribute__((weak))
# elif defined _MSC_VER
#  define MRBX_WEAK __declspec(selectany)
# else
#  define MRBX_WEAK
# endif
#endif

#ifndef MRBX_PRETTY_FUNCTION
# if defined(__GNUC__) || defined (__clang__)
#  define MRBX_PRETTY_FUNCTION __PRETTY_FUNCTION__
# elif defined _MSC_VER
#  define MRBX_PRETTY_FUNCTION __FUNCSIG__
# else
#  define MRBX_PRETTY_FUNCTION __func__
# endif
#endif

#ifndef MRBX_LITERAL_P
# if defined(__GNUC__) || defined (__clang__)
#  define MRBX_LITERAL_P(E) (__builtin_constant_p(E))
# else
#  define MRBX_LITERAL_P(E) (0)
# endif
#endif

#ifndef ELEMENTOF
# define ELEMENTOF(A)   (sizeof((A)) / sizeof((A)[0]))
#endif

#ifndef ENDOF
# define ENDOF(A)       ((A) + ELEMENTOF((A)))
#endif

#ifndef MRBX_SMALL_TARGET
# ifdef MRB_INT16
#  define MRBX_SMALL_TARGET 1
# endif
#endif

#include "compat/object.h"
#include <string.h>

#ifdef __cplusplus

MRBX_INLINE mrb_sym mrbx_symbol(mrb_state *mrb, mrb_value sym) { return mrb_symbol(sym); }
MRBX_INLINE mrb_sym mrbx_symbol(mrb_state *mrb, mrb_sym sym) { return sym; }
MRBX_INLINE mrb_sym mrbx_symbol(mrb_state *mrb, const char *sym) { return mrb_intern_cstr(mrb, sym); }

#else

MRBX_INLINE mrb_sym mrbx_symbol(mrb_state *mrb, mrb_value sym) { return mrb_symbol(sym); }
MRBX_INLINE mrb_sym mrbx_symbol_sym(mrb_state *mrb, mrb_sym sym) { return sym; }
MRBX_INLINE mrb_sym mrbx_intern_lit(mrb_state *mrb, const char *str) { return mrb_intern_static(mrb, str, strlen(str)); }

#   define MRBX_SYMBOL_CSTR_FUNC(CSTR)                                      \
        (MRBX_LITERAL_P(CSTR) ?                                             \
         mrbx_intern_lit :                                                  \
         mrb_intern_cstr)                                                   \

#   define mrbx_symbol(MRB, V)                                              \
        _Generic((V),                                                       \
                 mrb_value:     mrbx_symbol,                                \
                 mrb_sym:       mrbx_symbol_sym,                            \
                 const mrb_sym: mrbx_symbol_sym,                            \
                 char *:        MRBX_SYMBOL_CSTR_FUNC(V),                   \
                 const char *:  MRBX_SYMBOL_CSTR_FUNC(V))                   \
            (MRB, V)                                                        \

#endif

#define SYMBOL(SYM)     mrbx_symbol(mrb, (SYM))

#endif /* MRUBY_AUX_COMMON_H */
