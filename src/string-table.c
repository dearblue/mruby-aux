#include <mruby-aux/string.h>
#include <mruby-aux/utils.h>

MRB_API struct RString *
mrbx_str_new_table(mrb_state *mrb, struct RArray *list)
{
  size_t nelem = RARY_LEN(list);
  size_t memsize = sizeof(void *) * (nelem + 1); // NOTE: 終端に NULL を置くため +1 する

  FOREACH_RARRAY(*i, list) {
    mrb_check_type(mrb, *i, MRB_TT_STRING);
    // NOTE: mrb_str_strlen() は C の文字列として途中に NUL が紛れていないかも確認してくれる素敵な関数
    memsize += mrb_str_strlen(mrb, RSTRING(*i)) + 1; // NOTE: NUL 終端のために +1 する
  }

  struct RString *strpool = RSTRING(mrb_str_new(mrb, NULL, memsize));
  char **strtable = (char **)RSTR_PTR(strpool);
  char *str = (char *)(strtable + (nelem + 1));
  FOREACH_RARRAY(*i, list) {
    struct RString *s = RSTRING(*i);
    mrb_int len = RSTR_LEN(s);
    memcpy(str, RSTR_PTR(s), len);
    str[len] = '\0';
    *strtable++ = str;
    str += len + 1;
  }
  *strtable = NULL;

  return strpool;
}
