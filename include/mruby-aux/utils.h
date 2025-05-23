#ifndef MRUBY_AUX_UTILS_H
#define MRUBY_AUX_UTILS_H 1

#include "common.h"
#include "array.h"
#include "compat/mruby.h"
#include <mruby.h>
#include <mruby/value.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <mruby/gc.h>
#include "supplement.h"

#define IMPLEMENT_ME                                                    \
        mrb_raisef(mrb, E_NOTIMP_ERROR,                                 \
                   "IMPLEMENT ME! (in %S:%S:%S)",                       \
                   mrb_str_new_cstr(mrb, __FILE__),                     \
                   mrb_fixnum_value(__LINE__),                          \
                   mrb_str_new_cstr(mrb, __func__))                     \

#define FUNCALL(...)                    MRBX_FUNCALL(__VA_ARGS__)
#define FUNCALL_WITH_BLOCK(...)         MRBX_FUNCALL_WITH_BLOCK(__VA_ARGS__)

#define FOREACH_LIST(TYPE, I, ...)                                      \
    for (TYPE _list_[] = { __VA_ARGS__ },                               \
              *_list_end_ = _list_ + ELEMENTOF(_list_),                 \
              I = _list_;                                               \
         &I < _list_end_;                                               \
         &I++)                                                          \

#define FOREACH_ALIST(TYPE, I, LIST)                                    \
    for (TYPE *_list_end_ = (LIST) + ELEMENTOF((LIST)),                 \
              I = (LIST);                                               \
         &I < _list_end_;                                               \
         &I++)                                                          \

#define FOREACH_NLIST(TYPE, I, N, LIST)                                 \
    for (TYPE *_list_end_ = (LIST) + (N),                               \
              I = (LIST);                                               \
         &I < _list_end_;                                               \
         &I++)                                                          \

#define FOREACH_RARRAY(I, LIST)                                         \
    for (const mrb_value *_list_end_,                                   \
                         I = mrbx_expect_ary_ptr_getmem_termed(mrb, LIST, &_list_end_); \
         &I < _list_end_;                                               \
         &I++)                                                          \

MRBX_INLINE void *
mrbx_getrefp(mrb_state *mrb, mrb_value obj, const mrb_data_type *type)
{
  void *p;

  Data_Get_Struct(mrb, obj, type, p);

  return p;
}

MRBX_INLINE void *
mrbx_getref(mrb_state *mrb, mrb_value obj, const mrb_data_type *type)
{
  void *p = mrbx_getrefp(mrb, obj, type);

  if (!p) {
    mrb_raisef(mrb, E_TYPE_ERROR,
               "invalid initialized pointer - %S",
               obj);
  }

  return p;
}

MRBX_INLINE mrb_value
mrbx_funcall_passthrough(mrb_state *mrb, mrb_value recv, mrb_sym mid)
{
  mrb_int argc;
  mrb_value *argv, block;
  mrb_get_args(mrb, "*&", &argv, &argc, &block);
  return mrb_funcall_with_block(mrb, recv, mid, argc, argv, block);
}

MRBX_INLINE mrb_value
mrbx_instance_eval(mrb_state *mrb, mrb_value o, mrb_value b)
{
  return mrb_yield_with_class(mrb, b, 1, &b, o, NULL);
}

MRBX_INLINE mrb_value
mrbx_instance_exec(mrb_state *mrb, mrb_value o, mrb_value b, mrb_int argc, const mrb_value argv[])
{
  return mrb_yield_with_class(mrb, b, argc, argv, o, NULL);
}

#ifdef FOR_DOCUMENT_ONLY
/**
 * @def mrbx_get_const_cstr
 *
 * オブジェクトが保持している C の文字列ポインタを取得する。
 *
 * C のプロトタイプ宣言風に記述すると以下の通り:
 *  static const char *mrbx_get_const_cstr(mrb_state *mrb, mrb_value v);
 *  static const char *mrbx_get_const_cstr(mrb_state *mrb, struct RString *str);
 *  static const char *mrbx_get_const_cstr(mrb_state *mrb, mrb_sym sym);
 */
# define mrbx_get_const_cstr(mrb, var)
#endif

MRBX_INLINE const char *
mrbx_get_const_cstr_from_value(mrb_state *mrb, mrb_value v)
{
  if (mrb_symbol_p(v)) {
    return mrb_sym_name(mrb, mrb_symbol(v));
  } else if (mrb_string_p(v)) {
    return mrb_str_to_cstr(mrb, v);
  }

  if (!mrb_nil_p(v)) {
    mrb_raisef(mrb, E_TYPE_ERROR,
               "wrong value - %S (expect string, symbol or nil only)",
               v);
  }

  return NULL;
}

MRBX_INLINE const char *
mrbx_get_const_cstr_from_string(mrb_state *mrb, struct RString *str)
{
  if (str) {
    return mrb_str_to_cstr(mrb, mrb_obj_value(str));
  } else {
    return NULL;
  }
}

#ifdef __cplusplus

MRBX_INLINE const char *mrbx_get_const_cstr(mrb_state *mrb, mrb_value v) { return mrbx_get_const_cstr_from_value(mrb, v); }
MRBX_INLINE const char *mrbx_get_const_cstr(mrb_state *mrb, struct RString *str) { return mrbx_get_const_cstr_from_string(mrb, str); }
MRBX_INLINE const char *mrbx_get_const_cstr(mrb_state *mrb, mrb_sym sym) { return mrb_sym_name(mrb, sym); }

#elif __STDC_VERSION__ >= 201112L

# define mrbx_get_const_cstr(MRB, V)                                    \
         _Generic(V,                                                    \
                  mrb_value:         mrbx_get_const_cstr_from_value,    \
                  struct RString *:  mrbx_get_const_cstr_from_string,   \
                  mrb_sym:           mrb_sym_name                       \
         )(MRB, V)                                                      \

#endif

MRBX_INLINE void
mrbx_error_arity(mrb_state *mrb, mrb_int argc, mrb_int min, mrb_int max)
{
  if (max == min) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong number of arguments (given %S, expected %S)",
               mrb_fixnum_value(argc),
               mrb_fixnum_value(min));
  } else if (max < min) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong number of arguments (given %S, expected %S+)",
               mrb_fixnum_value(argc),
               mrb_fixnum_value(min));
  } else {
    mrb_raisef(mrb, E_ARGUMENT_ERROR,
               "wrong number of arguments (given %S, expected %S..%S)",
               mrb_fixnum_value(argc),
               mrb_fixnum_value(min),
               mrb_fixnum_value(max));
  }
}

MRB_BEGIN_DECL

MRB_API void mrbx_cleanup_data_objects(mrb_state *mrb, int num, const struct mrb_data_type *types[]);
MRB_API void mrbx_get_read_args(mrb_state *mrb, ssize_t *size, struct RString **dest);

/**
 * `mrb_cptr_value()` のラッパー関数です。
 * オブジェクトの確保に失敗した時は例外を発生させず、`mrb_nil_value()` を返します。
 */
MRB_API mrb_value mrbx_cptr_value(mrb_state *mrb, void *ptr);

MRB_API void mrbx_get_argset(mrb_state *mrb, mrb_int *argc, const mrb_value **argv, mrb_value *argkw, mrb_value *block);
MRB_API mrb_int mrbx_get_argc(mrb_state *mrb);
MRB_API const mrb_value *mrbx_get_argv(mrb_state *mrb);
MRB_API mrb_value mrbx_get_arg1(mrb_state *mrb);
MRB_API mrb_value mrbx_get_argblock(mrb_state *mrb);
MRB_API mrb_bool mrbx_given_block_p(mrb_state *mrb);

MRB_INLINE int mrbx_out_of_pointer_p(const char *str, const char *end);

MRB_INLINE int
mrbx_out_of_pointer_p(const char *str, const char *end)
{
  return (str == NULL || str >= end || (end - str) <= 0);
}

MRB_API void *mrbx_alloca(mrb_state *mrb, size_t size);

MRB_END_DECL

#endif /* MRUBY_AUX_UTILS_H */
