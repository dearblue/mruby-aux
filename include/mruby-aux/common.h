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

#endif /* MRUBY_AUX_COMMON_H */
