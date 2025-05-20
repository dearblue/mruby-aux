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

MRB_API mrb_bool mrbx_pathsep_p(int ch);
MRB_API mrb_bool mrbx_need_pathsep_p(const char *path, uint16_t len);
MRB_API mrbx_pathinfo mrbx_split_path(const char *path, uint16_t len);

MRB_END_DECL

#endif /* MRUBY_AUX_PATHINFO_H */
