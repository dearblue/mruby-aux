#ifndef MRUBY_AUX_PATHINFO_H
#define MRUBY_AUX_PATHINFO_H 1

#include <mruby.h>

MRB_BEGIN_DECL

typedef struct mrbx_pathinfo mrbx_pathinfo;

struct mrbx_pathinfo
{
  const char *path;
  uint16_t len;
  uint16_t rootterm;
  uint16_t dirterm;
  uint16_t basename;
  uint16_t extname;
  uint16_t nameterm;
};

MRB_API void mrbx_pathinfo_parse(mrbx_pathinfo *pinfo);
MRB_API const char *mrbx_pathinfo_root(const mrbx_pathinfo *pinfo, size_t *len);
MRB_API const char *mrbx_pathinfo_parent(const mrbx_pathinfo *pinfo, size_t *len);
MRB_API const char *mrbx_pathinfo_dirname(const mrbx_pathinfo *pinfo, size_t *len);
MRB_API const char *mrbx_pathinfo_basename(const mrbx_pathinfo *pinfo, size_t *len);
MRB_API const char *mrbx_pathinfo_extname(const mrbx_pathinfo *pinfo, size_t *len);


/**
 * 引数 `ch` がディレクトリ区切りかどうかを判定し、真偽値を返します。
 */
MRB_API mrb_bool mrbx_path_separator_p(int32_t ch);

/**
 * ディレクトリ区切り文字を末尾に追加する必要があるかを判定し、真偽値を返します。
 */
MRB_API mrb_bool mrbx_path_need_separator_p(const char path[], const char *end);

/**
 * 引数 `path` の先頭にあるルートディレクトリ構成要素を読み飛ばします。
 * ただし、例えば `/////` から始まったとしても読み飛ばされるのは最初の `/` だけです。
 */
MRB_API const char *mrbx_path_skip_root(const char path[], const char *end);

/**
 * 引数 `path` の先頭にあるディレクトリ区切り文字を読み飛ばします。
 * 複数の `/` が続く場合、まとめて読み飛ばします。
 */
MRB_API const char *mrbx_path_skip_separator(const char path[], const char *end);

/**
 * 引数 `path` に続くファイル名構成要素を読み飛ばします。
 *
 * - 値域 `[path, retval)` がファイル名構成要素を意味します。
 * - 値域 `[path, extname)` が拡張子を除くファイル名を意味します。
 * - 値域 `[extname, retval)` が拡張子部分を意味します。
 *
 * `extname` の値域は `(path, retval]` です。
 * `extname` と `retval` が等価の場合、拡張子はありません。
 */
MRB_API const char *mrbx_path_skip_name(const char path[], const char *end, const char **extname);

MRB_END_DECL

#endif /* MRUBY_AUX_PATHINFO_H */
