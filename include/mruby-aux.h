#ifndef MRUBY_AUX_H__
#define MRUBY_AUX_H__ 1

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

#if MRUBY_RELEASE_NO < 10200
#   define MRB_FROZEN_P(S)  FALSE
#elif MRUBY_RELEASE_NO < 10300
#   define MRB_FROZEN_P(S)  ((S)->tt == MRB_TT_STRING ? RSTR_FROZEN_P((S)) : FALSE)
#else
#endif

#define MRBX_OBJ_NEW(MRB, KLASS, ...)                       \
     mrb_obj_new(MRB, KLASS,                                \
             ELEMENTOF(((const VALUE []) { __VA_ARGS__ })), \
             (const VALUE []) { __VA_ARGS__ } )             \

static inline mrb_value
_aux_mrb_obj_value(mrb_state *mrb, void *v)
{
    return mrb_obj_value(v);
}

static inline mrb_value
_aux_to_value(mrb_state *mrb, mrb_value v)
{
    return v;
}

static inline mrb_value
_aux_mrb_fixnum_value(mrb_state *mrb, mrb_int v)
{
    return mrb_fixnum_value(v);
}

#define mrb_value(V)                                        \
    (_Generic((V),                                          \
              mrb_value:            _aux_to_value,          \
              struct RBasic *:      _aux_mrb_obj_value,     \
              struct RObject *:     _aux_mrb_obj_value,     \
              struct RClass *:      _aux_mrb_obj_value,     \
              struct RArray *:      _aux_mrb_obj_value,     \
              struct RString *:     _aux_mrb_obj_value,     \
              struct RProc *:       _aux_mrb_obj_value,     \
              struct RRange *:      _aux_mrb_obj_value,     \
              struct RFiber *:      _aux_mrb_obj_value,     \
              struct RException *:  _aux_mrb_obj_value,     \
              mrb_int:              _aux_mrb_fixnum_value,  \
              const mrb_int:        _aux_mrb_fixnum_value,  \
              mrb_float:            mrb_float_value,        \
              const mrb_float:      mrb_float_value,        \
              char *:               mrb_str_new_cstr,       \
              const char *:         mrb_str_new_cstr)       \
        (mrb, (V)))                                         \

static inline mrb_sym
_aux_mrb_sym(mrb_state *mrb, mrb_value sym)
{
    return mrb_symbol(sym);
}

static inline mrb_sym
_aux_symbol(mrb_state *mrb, mrb_sym sym)
{
    return sym;
}

#define SYMBOL(SYM)                             \
    _Generic((SYM),                             \
             mrb_value:     _aux_mrb_sym,       \
             mrb_sym:       _aux_symbol,        \
             const mrb_sym: _aux_symbol,        \
             char *:        mrb_intern_cstr,    \
             const char *:  mrb_intern_cstr)    \
        (mrb, (SYM))                            \

#include "mruby-aux/array.h"
#include "mruby-aux/string.h"
#include "mruby-aux/class.h"
#include "mruby-aux/utils.h"
#include "mruby-aux/throw.h"

#endif /* MRUBY_AUX_H__ */
