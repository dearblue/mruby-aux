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
# define MRB_METHOD_FUNC(M)         ((mrb_value (*)(mrb_state *, mrb_value))NULL)
# define MRB_METHOD_FROM_FUNC(M, F) do { (M) = mrb_proc_new_cfunc(mrb, F); } while (0)
# define MRB_METHOD_FROM_PROC(M, P) do { (M) = (P); } while (0)
# define MRB_METHOD_PROC_P(M)       (TRUE)
# define MRB_METHOD_PROC(M)         (M)
# define MRB_METHOD_UNDEF_P(M)      ((M) == NULL)
# define MRB_METHOD_CFUNC_P(M)      (MRB_PROC_CFUNC_P(M))
# define MRB_METHOD_CFUNC(M)        (MRB_PROC_CFUNC_P(M) ? (M)->body.func : NULL)

# ifndef MRB_PROC_SET_TARGET_CLASS
#  define MRB_PROC_SET_TARGET_CLASS(PROC, CLASS) ((PROC)->target_class = (CLASS))
# endif

# define MRBX_PROC_CFUNC(P)         ((P)->body.func)
#else
# define MRBX_PROC_CFUNC(P)         MRB_PROC_CFUNC(P)
#endif

#endif /* MRUBY_AUX_COMPAT_PROC_H */
