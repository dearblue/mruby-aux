/**
 *  @file include/mruby-aux/error.h
 */

#ifndef MRUBY_AUX_ERROR_H
#define MRUBY_AUX_ERROR_H 1

#include "common.h"
#include <mruby.h>
#include <mruby-aux/compat/error.h>
#include <mruby-aux/pp.h>

MRB_BEGIN_DECL

MRB_API mrb_value
mrbx_protect_exceptions(
        mrb_state *mrb,
        mrb_func_t body, mrb_value b_data,
        mrb_func_t rescue, mrb_value r_data,
        mrb_func_t ensure, mrb_value e_data,
        mrb_int len, struct RClass *classes[]);

/**
 *  `FUNC` を `mrb_protect_error()` 経由で呼び出し、その直後に必ずユーザーブロックを一度だけ実行します。
 *  `FUNC` 内で Ruby の例外のような大域ジャンプが発生した場合でもブロックは実行され、
 *  ブロック終了後に大域ジャンプが再送出されます。
 *  ただし `mrb->jmp` が `NULL` になっている状態 (`main()` 関数直下など) では、大域ジャンプは再送出されません。
 *
 *  ブロック内で `mrb->exc != NULL` をチェックすると、`FUNC` で大域ジャンプが発生したかを判定できます。
 *
 *  大域ジャンプを抑止して処理を継続したい場合は、`MRBX_ENSURE_BREAK()` を使ってください。
 *  大域ジャンプを `MRBX_ENSURE()` の外側で再送出させたい場合は、単純に `break` してください。
 *
 *  - `MRB`: mrb_state ポインタ
 *  - `RESULT_VAR`: 事前定義された mrb_value 型の変数 (`FUNC` の戻り値を受け取る)
 *  - `FUNC`: 呼び出す関数 (`mrb_protect_error_func` 互換)
 *  - `DATA`: `FUNC` に渡すユーザーデータ
 *
 *  Example:
 *
 *      mrb_value result;
 *      MRBX_ENSURE(mrb, result, body_func, userdata) {
 *        // このブロックは必ず実行される (Ruby の ensure 相当)
 *
 *        // 共通の後処理
 *
 *        if (mrb->exc) {
 *          // 大域ジャンプ発生時の後処理
 *        } else {
 *          // 正常完了時の後処理
 *        }
 *
 *        // 共通の後処理
 *
 *        // 大域ジャンプを無視する場合はここで `MRBX_ENSURE_BREAK()` を使う
 *      }
 */
#define MRBX_ENSURE(MRB, RESULT_VAR, FUNC, DATA) \
        for (mrb_bool MRBX_UNIQNAME(_break_) = FALSE; \
             !MRBX_UNIQNAME(_break_) && \
                (((RESULT_VAR) = mrb_protect_error(MRB, FUNC, DATA, &MRBX_UNIQNAME(_break_))), \
                 ((MRB)->exc = (MRBX_UNIQNAME(_break_) ? mrb_obj_ptr((RESULT_VAR)) : NULL)), \
                 TRUE); \
             (void)(MRBX_UNIQNAME(_break_) && (MRB)->jmp && (mrb_exc_raise(MRB, RESULT_VAR), TRUE)), \
                MRBX_UNIQNAME(_break_) = TRUE)

/**
 *  `MRBX_ENSURE()` のユーザーブロック内で大域ジャンプを無視します。
 *  これは大域ジャンプが発生していない場合でも有効です。
 *
 *  @note 実装の都合で `break` を使っています。
 *        ブロック内で `for/while` ループや `switch` を使う場合、干渉しないように注意してください。
 */
#define MRBX_ENSURE_BREAK(MRB) (MRB)->exc = NULL; break

MRB_END_DECL

#endif /* MRUBY_AUX_ERROR_H */
