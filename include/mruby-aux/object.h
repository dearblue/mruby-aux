#ifndef MRUBY_AUX_OBJECT_H
#define MRUBY_AUX_OBJECT_H 1

#include "compat/array.h"

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

#endif /* MRUBY_AUX_OBJECT_H */
