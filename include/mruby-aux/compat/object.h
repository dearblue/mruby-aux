#ifndef MRUBY_AUX_COMPAT_OBJECT_H
#define MRUBY_AUX_COMPAT_OBJECT_H 1

#include <mruby.h>
#include <mruby/object.h>
#include "../common.h"

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

MRBX_INLINE uint32_t
MRB_SET_FROZEN_FLAG(struct RObject *o)
{
    if (o->tt == MRB_TT_STRING) {
        return RSTR_SET_FROZEN_FLAG(o);
    } else {
        return o->flags;
    }
}

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

#  define MRB_FROZEN_P(O)                                                   \
       _Generic((O),                                                        \
                struct RBasic *:       mrbx_false_always,                   \
                struct RObject *:      mrbx_false_always,                   \
                struct RClass *:       mrbx_false_always,                   \
                struct RArray *:       mrbx_false_always,                   \
                struct RData *:        mrbx_false_always,                   \
                struct REnv *:         mrbx_false_always,                   \
                struct RException *:   mrbx_false_always,                   \
                struct RFiber *:       mrbx_false_always,                   \
                struct RHash *:        mrbx_false_always,                   \
                struct RProc *:        mrbx_false_always,                   \
                struct RRange *:       mrbx_false_always,                   \
                struct RString *:      mrbx_rstr_frozen_p)(O)               \

# endif /* __cplusplus */

#else
#endif /* MRUBY_RELEASE_NO */

/* RCLASS_SUPER は mruby-2.0 (dev) で削除 */
#include <mruby/class.h>
#ifndef RCLASS_SUPER
# define RCLASS_SUPER(O)  (mrb_class_ptr(O)->super)
#endif

#include "../object.h"

#endif /* MRUBY_AUX_COMPAT_OBJECT_H */
