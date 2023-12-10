#ifndef MRUBY_AUX_COMPAT_VALUE_H
#define MRUBY_AUX_COMPAT_VALUE_H 1

#include <mruby.h>

#if defined(MRB_WORD_BOXING)
# ifndef BOXWORD_FIXNUM_SHIFT
/* 2.1.0 までは定義がない */
#  define BOXWORD_FIXNUM_SHIFT 1
#  define BOXWORD_FIXNUM_FLAG 1
# endif
#endif /* MRB_WORD_BOXING */

#if !defined(MRB_TT_FIXNUM) /* for mruby-2.1.2 or older */
# define MRB_TT_INTEGER MRB_TT_FIXNUM
#endif

#endif /* MRUBY_AUX_COMPAT_VALUE_H */
