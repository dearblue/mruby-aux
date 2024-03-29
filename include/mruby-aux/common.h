#ifndef MRUBY_AUX_COMMON_H
#define MRUBY_AUX_COMMON_H 1

#include <mruby.h>
#include <sys/types.h> /* for ssize_t */

#ifndef ELEMENTOF
# define ELEMENTOF(A)   (sizeof((A)) / sizeof((A)[0]))
#endif

#ifndef ENDOF
# define ENDOF(A)       ((A) + ELEMENTOF((A)))
#endif

#ifdef __cplusplus
# include <memory>
# define MRBX_MOVE(E) ::std::move(E)
#else
# define MRBX_MOVE(E) E
#endif

#define MRBX_LIST(T, ...)                                               \
        ELEMENTOF(((T []){ __VA_ARGS__ })),                             \
        MRBX_MOVE(((T []){ __VA_ARGS__ }))                              \

#ifndef MRB_WITHOUT_FLOAT
# include <float.h>
# ifdef MRB_USE_FLOAT
#  define MRBX_FLOAT_MATN_DIG FLT_MANT_DIG
# else
#  define MRBX_FLOAT_MATN_DIG DBL_MANT_DIG
# endif

/* 与えられた N を浮動小数点数にした場合、桁落ちしていれば真を返す */
/* NOTE: キャストしているのは負数シフトで報告される警告を避けるため */
# define MRBX_FLOAT_OUT_OF_INTEGER_P(N) (!((N) < (1LL << MRBX_FLOAT_MATN_DIG) && (N) > (int64_t)((uint64_t)-1LL << MRBX_FLOAT_MATN_DIG)))
#endif

#ifndef __STDC_VERSION__
# define __STDC_VERSION__ 0
#endif

#ifndef mrbx_restrict
# ifdef mrb_restrict
#  define mrbx_restrict mrb_restrict
# elif defined(__GNUC__) || defined(__clang__)
#  define mrbx_restrict __restrict__
# elif defined _MSC_VER
#  define mrbx_restrict __declspec(restrict)
# else
#  define mrbx_restrict __restrict
# endif
#endif

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

#define MRBX_TOKEN_2(X) #X
#define MRBX_TOKEN_1(X) MRBX_TOKEN_2(X)
#define MRBX_TOKEN(X)   MRBX_TOKEN_1(X)

#ifndef MRBX_SMALL_TARGET
# ifdef MRB_INT16
#  define MRBX_SMALL_TARGET 1
# endif
#endif

#include "version.h"
#include "compat/object.h"
#include <string.h>

static inline mrb_sym mrbx_symbol_by_cstr(mrb_state *mrb, const char *str) { return mrb_intern_cstr(mrb, str); }
static inline mrb_sym mrbx_symbol_by_symbol(mrb_state *mrb, mrb_sym sym) { return sym; }
static inline mrb_sym mrbx_symbol_by_value(mrb_state *mrb, mrb_value sym) { return mrb_obj_to_sym(mrb, sym); }

#ifdef __cplusplus

static inline mrb_sym mrbx_symbol(mrb_state *mrb, mrb_sym sym) { return mrbx_symbol_by_symbol(mrb, sym); }
static inline mrb_sym mrbx_symbol(mrb_state *mrb, mrb_value sym) { return mrbx_symbol_by_value(mrb, sym); }
static inline mrb_sym mrbx_symbol(mrb_state *mrb, const char *sym) { return mrbx_symbol_by_cstr(mrb, sym); }

#elif __STDC_VERSION__ >= 201112L

static inline mrb_sym mrbx_intern_lit(mrb_state *mrb, const char *str) { return mrb_intern_static(mrb, str, strlen(str)); }

# define MRBX_SYMBOL_CSTR_FUNC(CSTR)                                    \
         (MRBX_LITERAL_P(CSTR) ?                                        \
          mrbx_intern_lit :                                             \
          mrbx_symbol_by_cstr)                                          \

# define mrbx_symbol(MRB, V)                                            \
         _Generic(V,                                                    \
                  mrb_value:     mrbx_symbol_by_value,                  \
                  mrb_sym:       mrbx_symbol_by_symbol,                 \
                  const mrb_sym: mrbx_symbol_by_symbol,                 \
                  char *:        MRBX_SYMBOL_CSTR_FUNC(V),              \
                  const char *:  MRBX_SYMBOL_CSTR_FUNC(V)               \
         )(MRB, V)                                                      \

#else

# define mrbx_symbol(MRB, SYM) mrbx_symbol_by_symbol(MRB, SYM)

#endif

#define SYMBOL(SYM)     mrbx_symbol(mrb, SYM)

#endif /* MRUBY_AUX_COMMON_H */
