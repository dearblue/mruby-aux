#ifndef MRUBY_AUX_COMMON_H
#define MRUBY_AUX_COMMON_H 1

#ifndef MRBX_UNUSED
# if __cplusplus
#  if __cplusplus >= 201703L
#   define MRBX_UNUSED [[maybe_unused]]
#  elif defined(__GNUC__) || defined (__clang__)
#   define MRBX_UNUSED __attribute__((unused))
#  else
#   define MRBX_UNUSED inline
#  endif
# elif defined(__GNUC__) || defined (__clang__)
#  define MRBX_UNUSED __attribute__((unused))
# elif __STDC_VERSION__ >= 199901L
#  define MRBX_UNUSED inline
# else
#  define MRBX_UNUSED
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

MRBX_UNUSED static mrb_bool mrbx_false_always(void *p) { return FALSE; }
MRBX_UNUSED static mrb_bool mrbx_rstr_frozen_p(struct RString *p) { return RSTR_FROZEN_P(p); }

# ifdef __cplusplus

#  define MRB_FROZEN_P(O)   mrbx_frozen_p(O)

MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct RArray *p) { return mrbx_false_always((void *)p); }
MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct RBasic *p) { return mrbx_false_always((void *)p); }
MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct RClass *p) { return mrbx_false_always((void *)p); }
MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct RData *p) { return mrbx_false_always((void *)p); }
MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct REnv *p) { return mrbx_false_always((void *)p); }
MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct RException *p) { return mrbx_false_always((void *)p); }
MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct RFiber *p) { return mrbx_false_always((void *)p); }
MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct RHash *p) { return mrbx_false_always((void *)p); }
MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct RObject *p) { return mrbx_false_always((void *)p); }
MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct RProc *p) { return mrbx_false_always((void *)p); }
MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct RRange *p) { return mrbx_false_always((void *)p); }
MRBX_UNUSED static mrb_bool mrbx_frozen_p(struct RString *p) { return mrbx_rstr_frozen_p((p)); }

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

static inline mrb_sym mrbx_symbol(mrb_state *mrb, mrb_value sym) { return mrb_symbol(sym); }
static inline mrb_sym mrbx_symbol(mrb_state *mrb, mrb_sym sym) { return sym; }
static inline mrb_sym mrbx_symbol(mrb_state *mrb, const char *sym) { return mrb_intern_cstr(mrb, sym); }

#else

static inline mrb_sym mrbx_symbol(mrb_state *mrb, mrb_value sym) { return mrb_symbol(sym); }
static inline mrb_sym mrbx_symbol_sym(mrb_state *mrb, mrb_sym sym) { return sym; }

#   define mrbx_symbol(MRB, V)                      \
        _Generic((V),                               \
                 mrb_value:     mrbx_symbol,        \
                 mrb_sym:       mrbx_symbol_sym,    \
                 const mrb_sym: mrbx_symbol_sym,    \
                 char *:        mrb_intern_cstr,    \
                 const char *:  mrb_intern_cstr     \
                )(MRB, V)                           \

#endif

#define SYMBOL(SYM)     mrbx_symbol(mrb, (SYM))

#endif /* MRUBY_AUX_COMMON_H */
