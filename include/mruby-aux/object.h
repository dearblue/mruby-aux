#ifndef MRUBY_AUX_OBJECT_H
#define MRUBY_AUX_OBJECT_H 1

#include "compat/array.h"
#include "compat/object.h"

MRBX_INLINE struct RObject *mrbx_ptr_to_ptr(mrb_state *mrb, void *p) { return (struct RObject *)p; }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, mrb_value o) { return mrb_immediate_p(o) ? NULL : mrb_obj_ptr(o); }

#if __cplusplus

MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RBasic *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RObject *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RClass *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RArray *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RHash *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RString *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RProc *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RRange *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RFiber *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RException *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RData *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }
MRBX_INLINE struct RObject *mrbx_obj_ptr(mrb_state *mrb, struct RIstruct *v) { return mrbx_ptr_to_ptr(mrb, (void *)v); }

#else

# define mrbx_obj_ptr(V)                                                    \
        _Generic((V),                                                       \
                 mrb_value:             mrbx_obj_ptr,                       \
                 struct RBasic *:       mrbx_ptr_to_ptr,                    \
                 struct RObject *:      mrbx_ptr_to_ptr,                    \
                 struct RClass *:       mrbx_ptr_to_ptr,                    \
                 struct RArray *:       mrbx_ptr_to_ptr,                    \
                 struct RHash *:        mrbx_ptr_to_ptr,                    \
                 struct RString *:      mrbx_ptr_to_ptr,                    \
                 struct RProc *:        mrbx_ptr_to_ptr,                    \
                 struct RRange *:       mrbx_ptr_to_ptr,                    \
                 struct RFiber *:       mrbx_ptr_to_ptr,                    \
                 struct RException *:   mrbx_ptr_to_ptr,                    \
                 struct RData *:        mrbx_ptr_to_ptr,                    \
                 struct RIstruct *:     mrbx_ptr_to_ptr)                    \
            (mrb, (V))                                                      \

#endif


MRBX_INLINE mrb_value mrbx_obj_freeze(mrb_value o) { if (!mrb_immediate_p(o)) { MRB_SET_FROZEN_FLAG(mrb_obj_ptr(o)); } return o; }
MRBX_INLINE void *mrbx_freeze_ptr(void *p) { if (p) { MRB_SET_FROZEN_FLAG((struct RObject *)p); } return p; }

#ifdef __cplusplus

MRBX_INLINE struct RBasic *mrbx_obj_freeze(struct RBasic *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RObject *mrbx_obj_freeze(struct RObject *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RClass *mrbx_obj_freeze(struct RClass *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RArray *mrbx_obj_freeze(struct RArray *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RHash *mrbx_obj_freeze(struct RHash *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RString *mrbx_obj_freeze(struct RString *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RProc *mrbx_obj_freeze(struct RProc *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RRange *mrbx_obj_freeze(struct RRange *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RFiber *mrbx_obj_freeze(struct RFiber *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RException *mrbx_obj_freeze(struct RException *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RData *mrbx_obj_freeze(struct RData *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RIstruct *mrbx_obj_freeze(struct RIstruct *p) { mrbx_freeze_ptr((void *)p); return p; }

#else

MRBX_INLINE struct RBasic *mrbx_freeze_basic(struct RBasic *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RObject *mrbx_freeze_object(struct RObject *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RClass *mrbx_freeze_class(struct RClass *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RArray *mrbx_freeze_array(struct RArray *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RHash *mrbx_freeze_hash(struct RHash *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RString *mrbx_freeze_string(struct RString *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RProc *mrbx_freeze_proc(struct RProc *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RRange *mrbx_freeze_range(struct RRange *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RFiber *mrbx_freeze_fiber(struct RFiber *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RException *mrbx_freeze_exception(struct RException *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RData *mrbx_freeze_data(struct RData *p) { mrbx_freeze_ptr((void *)p); return p; }
MRBX_INLINE struct RIstruct *mrbx_freeze_istruct(struct RIstruct *p) { mrbx_freeze_ptr((void *)p); return p; }

# define mrbx_obj_freeze(O)                                                 \
        _Generic((O),                                                       \
                 mrb_value:             mrbx_obj_freeze,                    \
                 struct RBasic *:       mrbx_freeze_basic,                  \
                 struct RObject *:      mrbx_freeze_object,                 \
                 struct RClass *:       mrbx_freeze_class,                  \
                 struct RArray *:       mrbx_freeze_array,                  \
                 struct RHash *:        mrbx_freeze_hash,                   \
                 struct RString *:      mrbx_freeze_string,                 \
                 struct RProc *:        mrbx_freeze_proc,                   \
                 struct RRange *:       mrbx_freeze_range,                  \
                 struct RFiber *:       mrbx_freeze_fiber,                  \
                 struct RException *:   mrbx_freeze_exception,              \
                 struct RData *:        mrbx_freeze_data,                   \
                 struct RIstruct *:     mrbx_freeze_istruct)                \
            ((O))                                                           \

#endif /* __cplusplus */

#endif /* MRUBY_AUX_OBJECT_H */
