/**
 *  @file mruby-aux/supplement.h
 */

#ifndef MRUBY_AUX_SUPPLEMENT_H
#define MRUBY_AUX_SUPPLEMENT_H 1

#include <mruby-aux/pp.h>

/**
 *  @def MRBX_TEXTIZE(...) -> string literal
 *
 *  一つ以上の引数をまとめて一つの改行で終わる文字列リテラルを生成します。
 *  マクロを与えた場合、展開されます。
 *
 *  @note タブ・改行を含めて一つ以上の空白は、プリプロセッサによって単一の空白に置き換えられます。
 *        改行を用いたい場合はエスケープ表記で `\n` を直接埋め込んで下さい。
 */
#define MRBX_TEXTIZE(...)               MRBX_IPP_TEXTIZE_1(__VA_ARGS__)

/**
 *  @def MRBX_FUNCALL(mrb_state *mrb, mrb_value recv, mrb_sym mid [, mrb_value arg]...) -> value
 *
 *  可変長引数を直接渡せる `mrb_funcall_id()` のようなものです。
 *  `mrb_funcall_with_block()` に直接置き換わります。
 */
#define MRBX_FUNCALL(MRB, RECV, ...)    MRBX_FUNCALL_WITH_BLOCK(MRB, RECV, __VA_ARGS__, mrb_nil_value())

/**
 *  @def MRBX_FUNCALL_WITH_BLOCK(mrb_state *mrb, mrb_value recv, mrb_sym mid [, mrb_value arg]..., block) -> value
 *
 *  可変長引数を直接渡せる `mrb_funcall_id()` のようなもので、ブロック引数も与えることが出来ます。
 *  `mrb_funcall_with_block()` に直接置き換わります。
 */
#define MRBX_FUNCALL_WITH_BLOCK(MRB, RECV, MID, ...) MRBX_FUNCALL_WITH_BLOCK_SELECTOR(__VA_ARGS__)(MRB, RECV, MID, __VA_ARGS__)

/// @cond

#define MRBX_FUNCALL_WITH_BLOCK_ARGDECL(N)      MRBX_PP_JOIN(mrb_value a, N),
#define MRBX_FUNCALL_WITH_BLOCK_ARGSET(N)       MRBX_PP_JOIN(a, N),

#define MRBX_FUNCALL_WITH_BLOCK_SELECTOR(...)   MRBX_FUNCALL_WITH_BLOCK_NAME_MAKE(MRBX_PP_COUNT_ARGS30(__VA_ARGS__))
#define MRBX_FUNCALL_WITH_BLOCK_NAME_MAKE(ARGC) MRBX_PP_JOIN(mrbx_funcall_with_block_arg, ARGC)

#define MRBX_FUNCALL_WITH_BLOCK_REALIZE(N)                              \
  static inline mrb_value                                               \
  MRBX_FUNCALL_WITH_BLOCK_NAME_MAKE(N)(                                 \
      mrb_state *mrb, mrb_value recv, mrb_sym mid,                      \
      MRBX_PP_ONE_TO(N, MRBX_FUNCALL_WITH_BLOCK_ARGDECL) mrb_value blk) \
  {                                                                     \
    MRBX_PP_CHOICE(N, (const mrb_value args[N] = { MRBX_PP_ONE_TO(N, MRBX_FUNCALL_WITH_BLOCK_ARGSET) };), ()) \
    return mrb_funcall_with_block(mrb, recv, mid, N, MRBX_PP_CHOICE(N, (args), (NULL)), blk); \
  }                                                                     \

MRBX_PP_ZERO_TO(30, MRBX_FUNCALL_WITH_BLOCK_REALIZE)

/// @endcond

#endif /* MRUBY_AUX_SUPPLEMENT_H */
