#ifndef MRUBY_AUX_COMPAT_PROC_H
#define MRUBY_AUX_COMPAT_PROC_H 1

#include <mruby.h>
#include <mruby/proc.h>

#if MRUBY_RELEASE_NO < 10400
# include <mruby/value.h>
# include <mruby/string.h>
# include <string.h>

typedef struct RProc *mrb_method_t;

# define MRB_METHOD_FUNC_P(M)       (FALSE)
# define MRB_METHOD_FUNC(M)         (NULL)

# define MRB_METHOD_FROM_FUNC(M, F)                               \
  do {                                                            \
    mrb_bug(mrb, "[%S:%S:%S] "                                    \
                 "Not available this mruby version! "             \
                 "Use `mrb_proc_new_cfunc()` instead "            \
                 "of `MRB_METHOD_FROM_FUNC()`.",                  \
            mrb_str_new_lit(mrb, __FILE__),                       \
            mrb_fixnum_value(mrb, __LINE__),                      \
            mrb_str_new_static(mrb, __func__, strlen(__func__))); \
  } while (0)                                                     \

# define MRB_METHOD_FROM_PROC(M, P) do { (M) = (P); } while (0)
# define MRB_METHOD_PROC_P(M)       (TRUE)
# define MRB_METHOD_PROC(M)         (M)
# define MRB_METHOD_UNDEF_P(M)      ((M) == NULL)
# define MRB_METHOD_CFUNC_P(M)      (MRB_PROC_CFUNC_P(M))
# define MRB_METHOD_CFUNC(M)        (MRB_PROC_CFUNC_P(M) ? (M)->body.func : NULL)

# ifndef MRB_PROC_SET_TARGET_CLASS
#  define MRB_PROC_SET_TARGET_CLASS(PROC, CLASS) ((PROC)->target_class = (CLASS))
# endif
#endif

#endif /* MRUBY_AUX_COMPAT_PROC_H */
