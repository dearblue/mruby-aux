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
 *  引数 `FUNC` を呼び出し、その結果を変数 `RESULT_VAR` に代入します。
 *
 *  `FUNC` の実行による大域ジャンプの発生にかかわらず、指定されたブロックが実行されます。
 *
 *  大域ジャンプが発生しなければ、コードの書かれた順序にそってブロックの外側が実行されます。
 *  大域ジャンプが発生すれば、ブロックが終わると大域ジャンプが再開され、ブロックの外側は実行されません。
 *
 *      mrb_value result;
 *      MRBX_ENSURE(mrb, result, func, userdata) {
 *        // func が正常に制御を返しても、例外などの帯域ジャンプが発生しても、常に実行される。
 *        // Ruby の ensure ブロックと同等。
 *
 *        ...
 *
 *        // 例外などの大域ジャンプが発生している場合、このブロックが普通に終わると大域ジャンプが再開される。
 *        // もし大域ジャンプを無視したい場合、`mrb->exc = NULL; break;` を行うとよい。
 *      }
 */
#define MRBX_ENSURE(MRB, RESULT_VAR, FUNC, DATA) \
        for (mrb_bool MRBX_TMPVAR(_break_) = FALSE; \
             !MRBX_TMPVAR(_break_) && \
                (((RESULT_VAR) = mrb_protect_error(MRB, (FUNC), (DATA), &MRBX_TMPVAR(_break_))), \
                 ((MRB)->exc = (MRBX_TMPVAR(_break_) ? mrb_obj_ptr((RESULT_VAR)) : NULL)), \
                 TRUE); \
             (void)(MRBX_TMPVAR(_break_) && (mrb_exc_raise(mrb, RESULT_VAR), TRUE)), \
                MRBX_TMPVAR(_break_) = TRUE)

MRB_END_DECL

#endif /* MRUBY_AUX_ERROR_H */
