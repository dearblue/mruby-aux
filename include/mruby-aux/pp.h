/**
 *  @file mruby-aux/pp.h
 */

#ifndef MRUBY_AUX_PP_H
#define MRUBY_AUX_PP_H 1

#include "version.h"
#include "internals/pp.h"

/**
 *  @def MRBX_PP_JOIN(X, Y)
 *  @def MRBX_PP_JOIN(X, Y, Z...)
 *
 *  X と Y を連結した字句に展開されます。
 *  内部的には `X ## Y` のようなものですが、X と Y は展開された後の字句となります。
 *
 *  与えられる引数の数は最大で10個です。
 */
#define MRBX_PP_JOIN(X, ...)            MRBX_IPP_JOIN_1(X, __VA_ARGS__)

/**
 *  @def MRBX_PP_CHOICE(COND, T, F)
 *
 *  `COND` が `0` 以外であれば `T` に置き換わり、`0` であれば `F` に展開されます。
 *
 *  @note `T` と `F` であたえるコードの断片は括弧 `()` で括る必要があります。
 */
#define MRBX_PP_CHOICE(COND, T, F)      MRBX_IPP_CHOICE_EXPAND_1(COND, T, F)

/**
 *  @def MRBX_PP_EMBODY_NEWER_OR(VER, CODE...)
 *
 *  `MRUBY_RELEASE_NO` として定義されるような 5 桁のバージョン番号と比較して、同じかより新しい場合に `CODE` を展開します。
 */
#define MRBX_PP_EMBODY_NEWER_OR(VER, ...)   MRBX_PP_CHOICE(MRBX_PP_JOIN(MRBX_VERSION_NEWER_OR_, VER, _P), (__VA_ARGS__), ())

/**
 *  @def MRBX_PP_EMBODY_OLDER_OR(VER, CODE...)
 *
 *  `MRUBY_RELEASE_NO` として定義されるような 5 桁のバージョン番号と比較して、同じかより古い場合に `CODE` を展開します。
 */
#define MRBX_PP_EMBODY_OLDER_OR(VER, ...)   MRBX_PP_CHOICE(MRBX_PP_JOIN(MRBX_VERSION_OLDER_OR_, VER, _P), (__VA_ARGS__), ())

/**
 *  @def MRBX_PP_EMBODY_NEWER_THAN(VER, CODE...)
 *
 *  `MRUBY_RELEASE_NO` として定義されるような 5 桁のバージョン番号と比較して、より新しい場合に `CODE` を展開します。
 */
#define MRBX_PP_EMBODY_NEWER_THAN(VER, ...) MRBX_PP_CHOICE(MRBX_PP_JOIN(MRBX_VERSION_OLDER_OR_, VER, _P), (), (__VA_ARGS__))

/**
 *  @def MRBX_PP_EMBODY_OLDER_THAN(VER, CODE...)
 *
 *  `MRUBY_RELEASE_NO` として定義されるような 5 桁のバージョン番号と比較して、より古い場合に `CODE` を展開します。
 */
#define MRBX_PP_EMBODY_OLDER_THAN(VER, ...) MRBX_PP_CHOICE(MRBX_PP_JOIN(MRBX_VERSION_NEWER_OR_, VER, _P), (), (__VA_ARGS__))

/**
 *  @def MRBX_PP_ONE_TO(N, F)
 *
 *  `for (int i = 1; i <= N; i++) { F(i); }` のような展開を行います。
 *  `N` の最大値は 50 です。
 */
#define MRBX_PP_ONE_TO(N, F)            MRBX_PP_JOIN(MRBX_IPP_ONE_TO_, N)(F)

/**
 *  @def MRBX_PP_ZERO_TO(N, F)
 *
 *  `for (int i = 0; i <= N; i++) { F(i); }` のような展開を行います。
 *  `N` の最大値は 50 です。
 */
#define MRBX_PP_ZERO_TO(N, F)           MRBX_PP_JOIN(MRBX_IPP_ZERO_TO_, N)(F)

/**
 *  @def MRBX_PP_ONE_FROM(N, F)
 *
 *  `for (int i = N; i >= 1; i--) { F(i); }` のような展開を行います。
 *  `N` の最大値は 50 です。
 */
#define MRBX_PP_ONE_FROM(N, F)          MRBX_PP_JOIN(MRBX_IPP_ONE_FROM_, N)(F)

/**
 *  @def MRBX_PP_ZERO_FROM(N, F)
 *
 *  `for (int i = N; i >= 0; i--) { F(i); }` のような展開を行います。
 *  `N` の最大値は 50 です。
 */
#define MRBX_PP_ZERO_FROM(N, F)         MRBX_PP_JOIN(MRBX_IPP_ZERO_FROM_, N)(F)

/**
 *  @def MRBX_PP_COUNT_ARGS30(...) -> int
 *
 *  与えられた引数のカンマを30個まで数えます。
 *  つまり `MRBX_PP_COUNT_ARGS30()` や `MRBX_PP_COUNT_ARGS30(arg0)` は 0 に展開されます。
 *
 *  @note 引数が30から40個の場合、30に展開される。その後コンパイラによってエラーが報告される。
 *
 *  @note 引数が40個以上の場合、第40引数がそのままの形で展開される。
 *        その後コンパイラによってエラーが報告されるか、未定義の挙動が起こる。
 */
#define MRBX_PP_COUNT_ARGS30(...)       MRBX_IPP_COUNT_ARGS30_1(__VA_ARGS__)

/**
 *  @def MRBX_PP_EXPAND(...)
 *
 *  丸括弧で囲まれた内容から、丸括弧を除去するために使います。
 *
 *  ```c
 *  MRBX_PP_EXPAND((123))   ->   123
 *  ```
 */
#define MRBX_PP_EXPAND(...)             __VA_ARGS__

#endif /* MRUBY_AUX_PP_H */
