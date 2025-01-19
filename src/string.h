#ifndef MRUBY_AUX_SRC_STRING_H
#define MRUBY_AUX_SRC_STRING_H 1

#include <mruby.h>
#include <mruby-aux/string.h>

MRB_BEGIN_DECL

#ifdef _WIN32
# include <windows.h>

struct RString *mrbx_win32_str_new_cstr_by_cp(mrb_state *mrb, const char *src, intptr_t srclen, UINT srccp, UINT destcp);
#endif

MRB_END_DECL

#endif // MRUBY_AUX_SRC_STRING_H
