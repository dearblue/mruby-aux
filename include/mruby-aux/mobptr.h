#ifndef MRUBY_AUX_MOBPTR_H
#define MRUBY_AUX_MOBPTR_H 1

/**
 * mob オブジェクト単位でポインタを結びつける GC 対象ポインタマネージャ。
 *
 * mob オブジェクトに関連付けられたポインタは、mob オブジェクトが GC によって回収・破棄される時に開放されます。
 * 解放する順序は、mob オブジェクトに結び付けられた逆順で実行されます (スタックのような順序です)。
 *
 * 解放する時の解放関数は、ポインタごとに定義することが可能です。
 *
 * すでにあるポインタを mob オブジェクトに結びつけるには、`mrbx_mob_push()` 関数を使います。
 * `mrbx_mob_push()` 関数を必ず成功させたい場合は、先立って例外が起きても問題ない区間で `mrbx_mob_order()` を使います。
 *
 * ```c
 * mrb_value mob = mrbx_mob_create(mrb);        // 最初に mob オブジェクトを作成します
 * void *ptr = my_ptr_new(...);
 * mrbx_mob_push(mrb, mob, ptr, my_ptr_free);   // ポインタを mob オブジェクトに結びつけます; ここで例外が発生すると ptr がリークする
 * ...
 * mrbx_mob_free(mrb, mob, ptr);                // ポインタを明示的に開放します
 * ```
 *
 * ```c
 * mrb_value mob = mrbx_mob_create(mrb);
 * mrbx_mob_order(mrb, mob, 1);                 // 1回分のポインタを確保する。失敗時は NoMemoryError
 * void *ptr = my_ptr_new(...);
 * mrbx_mob_push(mrb, mob, ptr, my_ptr_free);   // すでに1回分を確保してあるため、必ず成功する
 * ...
 * mrbx_mob_free(mrb, mob, ptr);
 * ```
 *
 * ----
 *
 * ポインタを切り離す場合は `mrbx_mob_pop()` 関数を使います。
 *
 * ```c
 * mrbx_mob_free_f *dfree = mrbx_mob_pop(mrb, mob, ptr);
 * ...
 * dfree(mrb, ptr);
 * ```
 *
 * ---
 *
 * mruby の `mrb_malloc()` / `mrb_calloc()` / `mrb_realloc()` / `mrb_malloc_simple()` / `mrb_realloc_simple()` 関数を模倣した関数が用意してあります。
 * これらは mob から切り離したら、使用者側で `mrb_free()` する必要があります。
 *
 * ```c
 * void *p = mrbx_mob_malloc(mrb, mob, 12345);
 * void *s = mrbx_mob_calloc(mrb, mob, 12345);
 * void *t = mrbx_mob_malloc(mrb, mob, 54321);
 * ...
 * mrbx_mob_free(mrb, mob, s);
 * ...
 * mrbx_mob_pop(mrb, mob, t);
 * ... // GC が起こって mob オブジェクトが破棄されたら p ポインタは開放されるが、t ポインタは生きたまま
 * ```
 */

#include <mruby.h>
#include <stddef.h>
#include <stdint.h>

typedef void mrbx_mob_free_f(mrb_state *mrb, void *ptr);

/**
 * mob オブジェクトを生成します。
 */
MRB_API mrb_value mrbx_mob_create(mrb_state *mrb);

/**
 * ポインタ登録のための予約を行います。
 * 続く `mrbx_mob_push()` 系や `mrbx_mob_alloc()` 系の関数は、少なくとも与えた `order` 引数の回数だけ登録のために失敗することがなくなります。
 *
 * 失敗した場合、`mrbx_mob_order()` は `NoMemoryError` 例外を発生させ、`mrbx_mob_order_noraise()` は `1` を返します。
 * どちらも成功時には `0` を返します。
 */
MRB_API int mrbx_mob_order(mrb_state *mrb, mrb_value mob, int order);
MRB_API int mrbx_mob_order_noraise(mrb_state *mrb, mrb_value mob, int order);

/**
 * 利用者側で得たポインタを mob オブジェクトに結びつけます。
 *
 * `mrbx_mob_push()` は、失敗すれば `NoMemoryError` 例外を発生させます。
 * `mrbx_mob_push_noraise()` は、失敗すれば `1` を返します。
 * どちらも成功時には `0` を返します。
 */
MRB_API int mrbx_mob_push(mrb_state *mrb, mrb_value mob, void *ptr, mrbx_mob_free_f *ptrfree);
MRB_API int mrbx_mob_push_noraise(mrb_state *mrb, mrb_value mob, void *ptr, mrbx_mob_free_f *ptrfree);

/**
 * mob オブジェクトから指定ポインタを切り離します。
 * 指定ポインタが不要になれば、戻り値である関数ポインタにより解放するべきです。
 */
MRB_API mrbx_mob_free_f *mrbx_mob_pop(mrb_state *mrb, mrb_value mob, void *ptr);

/**
 * 指定ポインタを開放します。
 * mob オブジェクトに結び付けられていない場合は何もしません。
 */
MRB_API void mrbx_mob_free(mrb_state *mrb, mrb_value mob, void *ptr);

/**
 * mob オブジェクトに結び付けられているポインタをすべて開放します。
 */
MRB_API void mrbx_mob_all_free(mrb_state *mrb, mrb_value mob);

/**
 * mob オブジェクトの空きスロットを纏め、可能な限り内部構造を開放します。
 */
MRB_API void mrbx_mob_compact(mrb_state *mrb, mrb_value mob);

/**
 * mob オブジェクトに結び付けられているポインタをすべて開放し、内部構造をすべて開放します。
 * GC による mob オブジェクトの破棄まで待たずに積極的に使うことが推奨されます。
 *
 * 再び mob オブジェクトとして活用することが可能です。
 */
MRB_API void mrbx_mob_cleanup(mrb_state *mrb, mrb_value mob);

/**
 * `mrb_malloc()` / `mrb_calloc()` / `mrb_realloc()` とほとんど同一です。
 * 得られたポインタはすべて mob オブジェクトに結び付けられます。
 *
 * ポインタが得られない場合や、結びつけのための内部領域の確保に失敗した場合は、実行時例外を発生させます。
 *
 * `mrbx_mob_realloc()` は指定ポインタが mob オブジェクトに結び付けられていない場合、実行時例外を発生させます。
 */
MRB_API void *mrbx_mob_malloc(mrb_state *mrb, mrb_value mob, size_t size);
MRB_API void *mrbx_mob_calloc(mrb_state *mrb, mrb_value mob, size_t num, size_t size);
MRB_API void *mrbx_mob_realloc(mrb_state *mrb, mrb_value mob, void *ptr, size_t size);

/**
 * `mrb_malloc_simple()` / `mrb_calloc_simple()` / `mrb_realloc_simple()` とほとんど同一です。
 * 得られたポインタはすべて mob オブジェクトに結び付けられます。
 *
 * ポインタが得られない場合や、結びつけのための内部領域の確保に失敗した場合は、NULL を返します。
 *
 * `mrbx_mob_realloc_simple()` と `mrbx_mob_reallocf_simple()` は、指定ポインタが mob オブジェクトに結び付けられていない場合、NULL を返します。
 *
 * `mrbx_mob_reallocf_simple()` は、ヒープの再確保に失敗した場合に `ptr` を開放して NULL を返します。
 */
MRB_API void *mrbx_mob_malloc_simple(mrb_state *mrb, mrb_value mob, size_t size);
MRB_API void *mrbx_mob_calloc_simple(mrb_state *mrb, mrb_value mob, size_t num, size_t size);
MRB_API void *mrbx_mob_realloc_simple(mrb_state *mrb, mrb_value mob, void *ptr, size_t size);
MRB_API void *mrbx_mob_reallocf_simple(mrb_state *mrb, mrb_value mob, void *ptr, size_t size);

#endif /* MRUBY_AUX_MOBPTR_H */
