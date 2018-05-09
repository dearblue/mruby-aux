#ifndef MRUBY_AUX_COMMON_H
#define MRUBY_AUX_COMMON_H 1

#include <string.h>

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

#include <mruby.h>

#if MRUBY_RELEASE_NO < 10200
# define MRB_FROZEN_P(S)  FALSE
#elif MRUBY_RELEASE_NO < 10300

# include <mruby/string.h>

struct RArray;
struct RBasic;
struct RClass;
struct RData;
struct REnv;
struct RException;
struct RFiber;
struct RHash;
struct RObject;
struct RProc;
struct RRange;
struct RString;

MRBX_INLINE mrb_bool mrbx_false_always(void *p) { return FALSE; }
MRBX_INLINE mrb_bool mrbx_rstr_frozen_p(struct RString *p) { return RSTR_FROZEN_P(p); }

# ifdef __cplusplus

#  define MRB_FROZEN_P(O)   mrbx_frozen_p(O)

MRBX_INLINE mrb_bool mrbx_frozen_p(struct RArray *p) { return mrbx_false_always((void *)p); }
MRBX_INLINE mrb_bool mrbx_frozen_p(struct RBasic *p) { return mrbx_false_always((void *)p); }
MRBX_INLINE mrb_bool mrbx_frozen_p(struct RClass *p) { return mrbx_false_always((void *)p); }
MRBX_INLINE mrb_bool mrbx_frozen_p(struct RData *p) { return mrbx_false_always((void *)p); }
MRBX_INLINE mrb_bool mrbx_frozen_p(struct REnv *p) { return mrbx_false_always((void *)p); }
MRBX_INLINE mrb_bool mrbx_frozen_p(struct RException *p) { return mrbx_false_always((void *)p); }
MRBX_INLINE mrb_bool mrbx_frozen_p(struct RFiber *p) { return mrbx_false_always((void *)p); }
MRBX_INLINE mrb_bool mrbx_frozen_p(struct RHash *p) { return mrbx_false_always((void *)p); }
MRBX_INLINE mrb_bool mrbx_frozen_p(struct RObject *p) { return mrbx_false_always((void *)p); }
MRBX_INLINE mrb_bool mrbx_frozen_p(struct RProc *p) { return mrbx_false_always((void *)p); }
MRBX_INLINE mrb_bool mrbx_frozen_p(struct RRange *p) { return mrbx_false_always((void *)p); }
MRBX_INLINE mrb_bool mrbx_frozen_p(struct RString *p) { return mrbx_rstr_frozen_p((p)); }

# else

#  define MRB_FROZEN_P(O)                                       \
       _Generic((O),                                            \
                struct RBasic *:       mrbx_false_always,       \
                struct RObject *:      mrbx_false_always,       \
                struct RClass *:       mrbx_false_always,       \
                struct RArray *:       mrbx_false_always,       \
                struct RData *:        mrbx_false_always,       \
                struct REnv *:         mrbx_false_always,       \
                struct RException *:   mrbx_false_always,       \
                struct RFiber *:       mrbx_false_always,       \
                struct RHash *:        mrbx_false_always,       \
                struct RProc *:        mrbx_false_always,       \
                struct RRange *:       mrbx_false_always,       \
                struct RString *:      mrbx_rstr_frozen_p)(O)   \

# endif /* __cplusplus */
#else
#endif /* MRUBY_RELEASE_NO */


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
