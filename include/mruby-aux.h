#ifndef MRUBY_AUX_H
#define MRUBY_AUX_H 1

#include "mruby-aux/common.h"
#include "mruby-aux/object.h"
#include "mruby-aux/array.h"
#include "mruby-aux/string.h"
#include "mruby-aux/class.h"
#include "mruby-aux/utils.h"
#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>

#define VALUE           mrb_value
#define Qnil            mrb_nil_value()
#define Qtrue           mrb_true_value()
#define Qfalse          mrb_false_value()
#define Qundef          mrb_undef_value()
#define NIL_P(obj)      mrb_nil_p(obj)
#define RTEST(O)        mrb_test(O)
#define mrb_cObject     (mrb->object_class)

#define MRB             mrb_state *mrb

#define MRBX_OBJ_NEW(MRB, KLASS, ...)                                       \
        mrb_obj_new(MRB, KLASS, MRBX_LIST(const mrb_value, __VA_ARGS__))    \

struct RIstruct;

MRBX_INLINE mrb_value mrbx_ptr_to_value(mrb_state *mrb, void *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
MRBX_INLINE mrb_value mrbx_value_to_value(mrb_state *mrb, mrb_value v) { return v; }
MRBX_INLINE mrb_value mrbx_fixnum_value(mrb_state *mrb, mrb_int v) { return mrb_fixnum_value(v); }

#ifdef __cplusplus

MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, mrb_value v) { return mrbx_value_to_value(mrb, v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RBasic *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RObject *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RClass *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RArray *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RHash *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RString *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RProc *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RRange *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RFiber *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RException *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RData *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, struct RIstruct *v) { return mrbx_ptr_to_value(mrb, (void *)v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, const mrb_int v) { return mrbx_fixnum_value(mrb, v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, const mrb_float v) { return mrb_float_value(mrb, v); }
MRBX_INLINE mrb_value mrbx_obj_value(mrb_state *mrb, const char *v) { return mrbx_value_str_new_cstr(mrb, v); }

# define mrb_value(V)   mrbx_obj_value(mrb, (V))

#else

# define MRBX_VALUE_STR_NEW_CSTR_FUNC(CSTR)                                 \
        (MRBX_LITERAL_P(CSTR) ?                                             \
         mrbx_value_str_new_lit :                                           \
         mrbx_value_str_new_cstr)                                           \

# define mrb_value(V)                                                       \
        _Generic((V),                                                       \
                 mrb_value:             mrbx_value_to_value,                \
                 struct RBasic *:       mrbx_ptr_to_value,                  \
                 struct RObject *:      mrbx_ptr_to_value,                  \
                 struct RClass *:       mrbx_ptr_to_value,                  \
                 struct RArray *:       mrbx_ptr_to_value,                  \
                 struct RHash *:        mrbx_ptr_to_value,                  \
                 struct RString *:      mrbx_ptr_to_value,                  \
                 struct RProc *:        mrbx_ptr_to_value,                  \
                 struct RRange *:       mrbx_ptr_to_value,                  \
                 struct RFiber *:       mrbx_ptr_to_value,                  \
                 struct RException *:   mrbx_ptr_to_value,                  \
                 struct RData *:        mrbx_ptr_to_value,                  \
                 struct RIstruct *:     mrbx_ptr_to_value,                  \
                 mrb_int:               mrbx_fixnum_value,                  \
                 const mrb_int:         mrbx_fixnum_value,                  \
                 mrb_float:             mrb_float_value,                    \
                 const mrb_float:       mrb_float_value,                    \
                 char *:                MRBX_VALUE_STR_NEW_CSTR_FUNC(V),    \
                 const char *:          MRBX_VALUE_STR_NEW_CSTR_FUNC(V))    \
            (mrb, (V))                                                      \

#endif

#endif /* MRUBY_AUX_H */
