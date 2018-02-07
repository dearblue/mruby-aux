#ifndef MRUBY_AUX_STRING_GROWUP_H_
#define MRUBY_AUX_STRING_GROWUP_H_ 1

#include "../string.h"

enum { MRBX_NEXT = -1, MRBX_STOP = -2, };

typedef ssize_t mrbx_str_growup(mrb_state *mrb, char *ptr, size_t *size, void *user);

/*
 * str.len から先を段階的に拡張していき、その拡張部分をその都度 func に渡す。
 * str の最初から行いたい場合は、あらかじめ RSTR_SET_LEN(str, 0) しておく。
 * func(mrb, ptr, available_and_usedsize, user)
 * 次に必要なサイズがあればそのバイト数を戻り値で示す。
 * MRBX_STOP で終了する。
 * MRBX_NEXT で処理を進める。
 */
struct RString *mrbx_str_buf_growup(mrb_state *mrb, struct RString *str, size_t maxsize, mrb_bool *is_partial, mrbx_str_growup *func, void *user);

#endif /* MRUBY_AUX_STRING_GROWUP_H_ */
