#ifndef MRBX_HASHARGS_H
#define MRBX_HASHARGS_H 1

#include "common.h"
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/hash.h>
#include "pp.h"

MRB_BEGIN_DECL

struct mrbx_scanhash_arg
{
  mrb_sym name;
  mrb_value *dest;
  mrb_value initval;
};

MRB_API mrb_value mrbx_scanhash(mrb_state *mrb, mrb_value hash, mrb_value rest, size_t argc, const struct mrbx_scanhash_arg *argv);


/**
 * メソッドが受け取るキーワード引数を解析します。
 *
 * マクロ引数はそれぞれが一度だけ評価されます (多重評価はされません)。
 *
 * 可変長部分の引数 (第4引数以降) の評価順は全ての環境で左から右に固定されます。
 *
 * 第1引数から第3引数の評価順は環境依存となります。
 *
 * @param hash
 *      解析対象のハッシュオブジェクト。nil の場合、受け取り変数を初期化するだけです。
 * @param rest
 *      解析後に残ったキーワードを受け取るハッシュオブジェクトを指定します。
 *      true を指定した場合、内部で新規ハッシュオブジェクトを用意します。
 *      NULL / false / nil の場合、任意キーワードの受け取りを認めません。
 * @param ...
 *      MRBX_SCANHASH_ARG が並びます。終端を表すものの記述は不要です。
 * @return
 *      このマクロ関数は文として展開されるため、返り値はありません。
 *
 * @sample
 *
 *  // 走査するハッシュオブジェクト
 *  mrb_value user_hash_object = mrb_hash_new(mrb);
 *
 *  mrb_value a, b, c, d, e, f; // これらの変数に受け取る
 *  MRBX_SCANHASH(mrb, user_hash_object, mrb_nil_value(),
 *          MRBX_SCANHASH_ARG("a", &a, mrb_nil_value()),
 *          MRBX_SCANHASH_ARG("b", &b, mrb_false_valse()),
 *          MRBX_SCANHASH_ARG("c", &c, mrb_str_new_cstr(mrb, "abcdefg")),
 *          MRBX_SCANHASH_ARG("d", &d, mrb_fixnum_value(5)));
 *
 * MRBX_SCANHASH_ARG の第2引数に NULL を与えると、名前の確認だけして、Cレベルの変数への代入は行わない。
 *          MRBX_SCANHASH_ARG("e", NULL, mrb_nil_value())
 *
 * MRBX_SCANHASH_ARG の第3引数に mrb_undef_value() を与えると、省略不可キーワード引数となる
 *          MRBX_SCANHASH_ARG("f", &f, mrb_undef_value())
 */
#define MRBX_SCANHASH(MRB, HASH, ...)                                   \
  do {                                                                  \
    const struct mrbx_scanhash_arg _mrbx_scanhash_args_[MRBX_IPP_SCANHASH_COUNT_KEY(__VA_ARGS__)] = MRBX_IPP_SCANHASH_KEYS(__VA_ARGS__); \
    mrbx_scanhash(MRB, HASH, MRBX_IPP_SCANHASH_REST(__VA_ARGS__),       \
                  MRBX_IPP_SCANHASH_COUNT_KEY(__VA_ARGS__),             \
                  _mrbx_scanhash_args_);                                \
  } while (0)                                                           \

/*
 * 評価順は左から右に固定される。
 *
 * [name]   C の文字列、または mrb_sym としてのシンボルを与える
 * [dest]   キーワード引数の代入先。NULL を指定した場合、名前の確認だけして、Cレベルの変数への代入は行わない
 * [vdefault] 規定値。Qundef を指定した場合、省略不可キーワードとなる
 */
#define MRBX_SCANHASH_ARG(NAME, DEST, VDEFAULT) { SYMBOL(NAME), (DEST), (VDEFAULT), }

#define MRBX_SCANHASH_ARGS(NAME, DEST, VDEFAULT) { MRBX_INTERN_LIT(NAME), (DEST), (VDEFAULT) }
#define MRBX_SCANHASH_ARGI(NAME, DEST, VDEFAULT) MRBX_SCANHASH_ARG((NAME), (DEST), (VDEFAULT))

#define MRBX_INTERN_LIT(STR) mrb_intern_lit(mrb, STR)

MRB_END_DECL


//// 内部実装

#define MRBX_IPP_SCANHASH_COUNT_KEY(...)           MRBX_IPP_SCANHASH_COUNT_KEY1(__VA_ARGS__)
#define MRBX_IPP_SCANHASH_KEYS(...)                MRBX_IPP_SCANHASH_KEYS1(__VA_ARGS__)
#define MRBX_IPP_SCANHASH_KEYS1(REST, ...)         { __VA_ARGS__ }
#define MRBX_IPP_SCANHASH_REST(...)                MRBX_IPP_SCANHASH_REST1(__VA_ARGS__, _)
#define MRBX_IPP_SCANHASH_REST1(REST, ...)         REST

#define MRBX_IPP_SCANHASH_COUNT_KEY1(...)                               \
  MRBX_IPP_SCANHASH_COUNT_KEY2(__VA_ARGS__,                             \
                               33, 33, 33, 32, 32, 32, 31, 31, 31, 30, 30, 30, 29, 29, 29, \
                               28, 28, 28, 27, 27, 27, 26, 26, 26, 25, 25, 25, 24, 24, 24, \
                               23, 23, 23, 22, 22, 22, 21, 21, 21, 20, 20, 20, 19, 19, 19, \
                               18, 18, 18, 17, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, \
                               13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 10,  9,  9,  9, \
                                8,  8,  8,  7,  7,  7,  6,  6,  6,  5,  5,  5,  4,  4,  4, \
                                3,  3,  3,  2,  2,  2,  1,  1,  1,  0, [[<<>>]]) \

#define MRBX_IPP_SCANHASH_COUNT_KEY2(_99, _98, _97, _96, _95, _94, _93, _92, _91, _90, \
                                     _89, _88, _87, _86, _85, _84, _83, _82, _81, _80, \
                                     _79, _78, _77, _76, _75, _74, _73, _72, _71, _70, \
                                     _69, _68, _67, _66, _65, _64, _63, _62, _61, _60, \
                                     _59, _58, _57, _56, _55, _54, _53, _52, _51, _50, \
                                     _49, _48, _47, _46, _45, _44, _43, _42, _41, _40, \
                                     _39, _38, _37, _36, _35, _34, _33, _32, _31, _30, \
                                     _29, _28, _27, _26, _25, _24, _23, _22, _21, _20, \
                                     _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, \
                                     _09, _08, _07, _06, _05, _04, _03, _02, _01, _00, N, ...) N \

#endif /* MRBX_HASHARGS_H */
