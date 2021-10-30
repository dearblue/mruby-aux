/**
 * @file include/mruby-aux/option.h
 *
 * `getopt(3)` と Ruby の `optparse` を参考にしています。
 */

#ifndef MRUBY_AUX_OPTION_H
#define MRUBY_AUX_OPTION_H 1

#include <mruby.h>
#include <stdarg.h>

typedef struct mrbx_option_entry mrbx_option_entry;
typedef struct mrbx_option mrbx_option;
typedef struct mrbx_option_vprintf mrbx_option_vprintf;
typedef int mrbx_option_vprintf_func(void *user, const char *fmt, va_list va);

struct mrbx_option_entry
{
  /**
   * 普通の (短い) オプションとなる文字コードです。
   *
   * 負の整数を指定した場合、`mrbx_option_usage()` で出力されない隠しオプションとなります。
   * `mrbx_option_parse()` の戻り値は `-opt` となるため、正の整数を返します。
   * `opt = -'A'` とした場合、コマンドラインで `-A` が指定されると、`mrbx_option_parse()` は `'A'` を返します。
   *
   * 普通の (短い) オプションを設定しない場合、`'\0'` を設定します。
   */
  int opt;

  /**
   * オプション引数を受け付ける場合、引数名として `mrbx_option_usage()` で出力される文字列を設定します。
   *
   */
  const char *arg;
  const char *optlong;
  const char *summary;
};

struct mrbx_option
{
  /** `main()` 関数であれば、`argv[0]` を渡すことが出来ます。 */
  const char *progname;

  /** `main()` 関数であれば、`argc - 1` を渡すことが出来ます。 */
  int argc;

  /** `main()` 関数であれば、`argv + 1` を渡すことが出来ます。 */
  char **argv;

  /** `entries` で渡すオプション要素の数です。 */
  int optnum;

  /** オプション要素です。 */
  const mrbx_option_entry *entries;

  /**
   * `NULL` で初期化する必要があります。
   *
   * `mrbx_option_parse()` が成功した場合、利用者が有効な値を得ることは出来ません。
   */
  const char *opt;

  /**
   * `mrbx_option_parse()` は処理の途中で代入するだけですが、`NULL` で初期化しておいたほうが無難です。
   *
   * 引数を取るオプションであれば、取得した文字列を取得できます。
   * それ以外は `NULL` です。
   */
  const char *arg;
};

struct mrbx_option_vprintf
{
  mrbx_option_vprintf_func *vprintf_func;
  void *user;
};

/**
 * @param (print)
 *      出力関数を変更できます。
 *      NULL を与えると、`vprintf()` 関数が呼ばれます。
 *      `MRB_DISABLE_STDIO` が設定された場合は NULL を与えると何も出力しません。
 *
 * 可変長引数は `header` と `footer` 両方で使えます。`printf` 書式として `header` で使われた引数を持ち越して `footer` で使うことは出来ません。
 */
MRB_API void mrbx_option_usage(const mrbx_option *opt, int width, int indent, const mrbx_option_vprintf *print, const char *header, const char *footer, ...);

/**
 * @retval >0
 *      正常にオプションが取得できたことを意味します。`mrbx_option_entry::opt` の絶対値を返します。
 *
 * @retval MRBX_OPTION_END
 *      入力の終わりに到達し、オプション解析を正常に終了しました。
 *      これ以上のオプションはありません (`argc == 0`)。
 *
 * @retval MRBX_OPTION_BREAK
 *      オプション以外の入力によってオプション解析を正常に完了しました。
 *
 * @retval MRBX_OPTION_STOP
 *      `"--"` によってオプション解析を正常に完了しました。
 *
 * @retval MRBX_OPTION_UNKNOWN
 *      与えられたオプションが未知であることを意味します。<br>
 *      `mrbx_option o` である場合、`*o.opt` によってエラーとなったオプション文字を取得できます。<br>
 *      もし `mrbx_option_parse()` を継続したい場合、`o.opt++` して処理を続けて下さい。
 *
 * @retval MRBX_OPTION_UNKNOWN_LONG
 *      与えられたオプションが未知であることを意味します。<br>
 *      `mrbx_option o` である場合、`o.opt` によってエラーとなったオプション文字列を取得できます。
 *      ※注意※オプションが "=" 付きで引数を与えられている場合、"=" 以降も含まれます。<br>
 *      もし `mrbx_option_parse()` を継続したい場合、`o.opt = NULL` して処理を続けて下さい。
 */
MRB_API int mrbx_option_parse(mrbx_option *opt);

#define MRBX_OPTION_END               0
#define MRBX_OPTION_BREAK            -1
#define MRBX_OPTION_STOP             -2
#define MRBX_OPTION_UNKNOWN         -10
#define MRBX_OPTION_UNKNOWN_LONG    -11

#define MRBX_OPTION_ENTRY(OPT, ARG, LONG, SUMMARY) { (OPT), (ARG), (LONG), (SUMMARY) }
#define MRBX_OPTION_LABEL(LABEL) { 0, NULL, NULL, (LABEL) }

#define MRBX_OPTION_SETUP(ARGC, ARGV, NUM_ENTRIES, ENTRIES)             \
  MRBX_OPTION_SETUP_WITH_NAME((ARGV)[0], (ARGC) - 1, (ARGV) + 1, (NUM_ENTRIES), (ENTRIES)) \

#define MRBX_OPTION_SETUP_WITH_NAME(NAME, ARGC, ARGV, NUM_ENTRIES, ENTRIES) \
  { (NAME), (ARGC), (ARGV), (NUM_ENTRIES), (ENTRIES), NULL, NULL }      \

#endif /* MRUBY_AUX_OPTION_H */
