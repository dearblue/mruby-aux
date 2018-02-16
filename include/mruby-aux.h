#ifndef MRUBY_AUX_H__
#define MRUBY_AUX_H__ 1

#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include <mruby/data.h>

#ifndef MRBX_UNUSED
#   if __cplusplus
#       if __cplusplus >= 201703L
#           define MRBX_UNUSED [[maybe_unused]]
#       elif defined(__GNUC__) || defined (__clang__)
#           define MRBX_UNUSED __attribute__((unused))
#       else
#           define MRBX_UNUSED inline
#       endif
#   elif defined(__GNUC__) || defined (__clang__)
#       define MRBX_UNUSED __attribute__((unused))
#   elif __STDC_VERSION__ >= 199901L
#       define MRBX_UNUSED inline
#   else
#       define MRBX_UNUSED
#   endif
#endif

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
#   define MRB_FROZEN_P(S)  (mrb_type((S)) == MRB_TT_STRING ? RSTR_FROZEN_P((S)) : FALSE)
#else
#endif

#define MRBX_OBJ_NEW(MRB, KLASS, ...)                       \
     mrb_obj_new(MRB, KLASS,                                \
             ELEMENTOF(((const VALUE []) { __VA_ARGS__ })), \
             (const VALUE []) { __VA_ARGS__ } )             \

struct RIstruct;

#if __cplusplus

static inline mrb_value _mrbx_obj_value(mrb_state *mrb, mrb_value v) { return v; }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, struct RBasic *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, struct RObject *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, struct RClass *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, struct RArray *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, struct RString *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, struct RProc *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, struct RRange *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, struct RFiber *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, struct RException *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, struct RData *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, struct RIstruct *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, const mrb_int v) { return mrb_fixnum_value(v); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, const mrb_float v) { return mrb_float_value(mrb, v); }
static inline mrb_value _mrbx_obj_value(mrb_state *mrb, const char *v) { return mrb_str_new_cstr(mrb, v); }

#   define _mrbx_obj_value(V)   _mrbx_obj_value

#else

static inline mrb_value _mrbx_obj_value(mrb_state *mrb, void *v) { return (v ? mrb_obj_value(v) : mrb_nil_value()); }
static inline mrb_value _mrbx_value_to_value(mrb_state *mrb, mrb_value v) { return v; }
static inline mrb_value _mrbx_fixnum_value(mrb_state *mrb, mrb_int v) { return mrb_fixnum_value(v); }

#   define _mrbx_obj_value(V)                                   \
        _Generic((V),                                           \
                  mrb_value:            _mrbx_value_to_value,   \
                  struct RBasic *:      _mrbx_obj_value,        \
                  struct RObject *:     _mrbx_obj_value,        \
                  struct RClass *:      _mrbx_obj_value,        \
                  struct RArray *:      _mrbx_obj_value,        \
                  struct RString *:     _mrbx_obj_value,        \
                  struct RProc *:       _mrbx_obj_value,        \
                  struct RRange *:      _mrbx_obj_value,        \
                  struct RFiber *:      _mrbx_obj_value,        \
                  struct RException *:  _mrbx_obj_value,        \
                  struct RData *:       _mrbx_obj_value,        \
                  struct RIstruct *:    _mrbx_obj_value,        \
                  mrb_int:              _mrbx_fixnum_value,     \
                  const mrb_int:        _mrbx_fixnum_value,     \
                  mrb_float:            mrb_float_value,        \
                  const mrb_float:      mrb_float_value,        \
                  char *:               mrb_str_new_cstr,       \
                  const char *:         mrb_str_new_cstr)       \

#endif

#define mrb_value(V)    _mrbx_obj_value((V))(mrb, (V))

#ifdef __cplusplus

static inline mrb_sym _mrbx_symbol(mrb_state *mrb, mrb_value sym) { return mrb_symbol(sym); }
static inline mrb_sym _mrbx_symbol(mrb_state *mrb, mrb_sym sym) { return sym; }
static inline mrb_sym _mrbx_symbol(mrb_state *mrb, const char *sym) { return mrb_intern_cstr(mrb, sym); }

#   define _mrbx_symbol(V)  _mrbx_symbol

#else

static inline mrb_sym _mrbx_symbol(mrb_state *mrb, mrb_value sym) { return mrb_symbol(sym); }
static inline mrb_sym _mrbx_symbol_sym(mrb_state *mrb, mrb_sym sym) { return sym; }

#   define _mrbx_symbol(V)                          \
        _Generic((V),                               \
                 mrb_value:     _mrbx_symbol,       \
                 mrb_sym:       _mrbx_symbol_sym,   \
                 const mrb_sym: _mrbx_symbol_sym,   \
                 char *:        mrb_intern_cstr,    \
                 const char *:  mrb_intern_cstr)    \

#endif

#define SYMBOL(SYM)     _mrbx_symbol(SYM)(mrb, (SYM))

#include "mruby-aux/array.h"
#include "mruby-aux/string.h"
#include "mruby-aux/class.h"
#include "mruby-aux/utils.h"
#include "mruby-aux/throw.h"

#endif /* MRUBY_AUX_H__ */
