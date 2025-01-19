#ifndef MRUBY_AUX_COMPAT_PROC_H
#define MRUBY_AUX_COMPAT_PROC_H 1

#include <mruby.h>
#include <mruby/proc.h>

#if MRUBY_RELEASE_NO < 10300
void mrb_env_unshare(mrb_state *mrb, struct REnv *e);
#endif

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

# define MRB_PROC_ENV(P)            ((P)->env)

# define MRBX_PROC_CFUNC(P)         ((P)->body.func)
#else
# define MRBX_PROC_CFUNC(P)         MRB_PROC_CFUNC(P)
#endif

#if MRUBY_RELEASE_NO < 20100
# define MRB_METHOD_NOARG_P(M)      FALSE
# define MRB_METHOD_NOARG_SET(M)    do { } while (0)
#endif

#if MRUBY_RELEASE_NO < 30000
static inline void
mrb_vm_ci_proc_set(mrb_callinfo *ci, const struct RProc *proc)
{
  ci->proc = (struct RProc *)(uintptr_t)proc;
  ci[1].pc = (proc && !MRB_PROC_CFUNC_P(proc)) ? proc->body.irep->iseq : NULL;
}

static inline struct RClass *
mrb_vm_ci_target_class(mrb_callinfo *ci)
{
  return ci->target_class;
}

static inline struct REnv *
mrb_vm_ci_env(mrb_callinfo *ci)
{
  return ci->env;
}

static inline void
mrb_vm_ci_env_set(mrb_callinfo *ci, struct REnv *env)
{
  ci->env = env;
}
#endif

#endif /* MRUBY_AUX_COMPAT_PROC_H */
