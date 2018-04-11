/*
 * This code is under public domain (CC0)
 * <https://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * To the extent possible under law, dearblue has waived all copyright
 * and related or neighboring rights to this work.
 *
 *     dearblue <dearblue@users.noreply.github.com>
 */

#ifndef MRBX_HASHARGS_H__
#define MRBX_HASHARGS_H__ 1

#include <mruby.h>
#include <mruby/array.h>
#include <mruby/hash.h>

#define MRBX_SCANHASH_ELEMENTOF(v)  (sizeof((v)) / sizeof((v)[0]))
#define MRBX_SCANHASH_ENDOF(v)      ((v) + MRBX_SCANHASH_ELEMENTOF((v)))

MRB_BEGIN_DECL

struct mrbx_scanhash_arg
{
    mrb_sym name;
    mrb_value *dest;
    mrb_value initval;
};

mrb_value mrbx_scanhash(mrb_state *mrb, mrb_value hash, mrb_value rest, size_t argc, struct mrbx_scanhash_arg *argv);


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
 *      MRBX_SCANHASH_ARG[SI] が並びます。終端を表すものの記述は不要です。
 * @return
 *      受け取り対象外のハッシュオブジェクト (rest で与えたもの) が返ります。
 *
 * @sample
 *
 *  // 走査するハッシュオブジェクト
 *  mrb_value user_hash_object = mrb_hash_new(mrb);
 *
 *  mrb_value a, b, c, d, e, f; // これらの変数に受け取る
 *  MRBX_SCANHASH(mrb, user_hash_object, mrb_nil_value(),
 *          MRBX_SCANHASH_ARGS("a", &a, mrb_nil_value()),
 *          MRBX_SCANHASH_ARGS("b", &b, mrb_false_valse()),
 *          MRBX_SCANHASH_ARGS("c", &c, mrb_str_new_cstr(mrb, "abcdefg")),
 *          MRBX_SCANHASH_ARGS("d", &d, mrb_fixnum_value(5)));
 *
 * MRBX_SCANHASH_ARG 系の第2引数に NULL を与えると、名前の確認だけして、Cレベルの変数への代入は行わない。
 *          MRBX_SCANHASH_ARGS("e", NULL, mrb_nil_value())
 *
 * MRBX_SCANHASH_ARG 系の第3引数に mrb_undef_value() を与えると、省略不可キーワード引数となる
 *          MRBX_SCANHASH_ARGS("f", &f, mrb_undef_value())
 */
#define MRBX_SCANHASH(mrb, hash, rest, ...)                                             \
    mrbx_scanhash(mrb, (hash), (rest),                                                  \
            MRBX_SCANHASH_ELEMENTOF(((struct mrbx_scanhash_arg []){ __VA_ARGS__ })),    \
            ((struct mrbx_scanhash_arg []){ __VA_ARGS__ }));                            \

/*
 * 評価順は左から右に固定される。
 *
 * [name]   C の文字列、または mrb_sym としてのシンボルを与える
 * [dest]   キーワード引数の代入先。NULL を指定した場合、名前の確認だけして、Cレベルの変数への代入は行わない
 * [vdefault] 規定値。Qundef を指定した場合、省略不可キーワードとなる
 */
#define MRBX_SCANHASH_ARG(name, dest, vdefault) { MRBX_SCANHASH_TO_SYMBOL(mrb, (name)), (dest), (vdefault), }

#define MRBX_SCANHASH_ARGS(name, dest, vdefault) MRBX_SCANHASH_ARG((name), (dest), (vdefault))
#define MRBX_SCANHASH_ARGI(name, dest, vdefault) MRBX_SCANHASH_ARG((name), (dest), (vdefault))

MRB_END_DECL

#include "scanhash/_to_sym.h"

#endif /* MRBX_HASHARGS_H__ */
