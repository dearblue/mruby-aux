#ifndef MRUBY_AUX_COMMON_H
#define MRUBY_AUX_COMMON_H 1

#if __cplusplus
# if __cplusplus >= 201703L
#  ifndef MRBX_UNUSED
#   define MRBX_UNUSED [[maybe_unused]]
#  endif
# elif defined(__GNUC__) || defined (__clang__)
#  ifndef MRBX_UNUSED
#   define MRBX_UNUSED __attribute__((unused))
#  endif
# else
#  ifndef MRBX_UNUSED
#   define MRBX_UNUSED
#  endif
# endif

# ifndef MRBX_INLINE
#  define MRBX_INLINE MRBX_UNUSED static inline
# endif

#elif defined(__GNUC__) || defined (__clang__)
# ifndef MRBX_UNUSED
#  define MRBX_UNUSED __attribute__((unused))
# endif

# ifndef MRBX_INLINE
#  define MRBX_INLINE MRBX_UNUSED static inline
# endif

#elif __STDC_VERSION__ >= 199901L
# ifndef MRBX_UNUSED
#  define MRBX_UNUSED
# endif

# ifndef MRBX_INLINE
#  define MRBX_INLINE MRBX_UNUSED static inline
# endif
#else
# ifndef MRBX_UNUSED
#  define MRBX_UNUSED
# endif

# ifndef MRBX_INLINE
#  define MRBX_INLINE MRBX_UNUSED static
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

#   define MRBX_SYMBOL_CSTR_FUNC(CSTR)  \
        (MRBX_LITERAL_P(CSTR) ?         \
         mrbx_intern_lit :              \
         mrb_intern_cstr)               \

#   define mrbx_symbol(MRB, V)                              \
        _Generic((V),                                       \
                 mrb_value:     mrbx_symbol,                \
                 mrb_sym:       mrbx_symbol_sym,            \
                 const mrb_sym: mrbx_symbol_sym,            \
                 char *:        mrb_intern_cstr,            \
                 const char *:  MRBX_SYMBOL_CSTR_FUNC(V))   \
            (MRB, V)                                        \

#endif

#define SYMBOL(SYM)     mrbx_symbol(mrb, (SYM))

#endif /* MRUBY_AUX_COMMON_H */
