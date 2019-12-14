#ifndef MRUBY_AUX_COMPONENT_NAME_H
#define MRUBY_AUX_COMPONENT_NAME_H 1

#include <stddef.h>
#include <stdbool.h>
#include <mruby.h>

MRB_BEGIN_DECL

typedef struct mrbx_component_name mrbx_component_name;

struct mrbx_component_name
{
  const char *path;
  const char *rootterm;
  const char *dirterm;
  const char *basename;
  const char *extname;
  const char *nameterm;
};

MRB_API bool mrbx_pathsep_p(int ch);
MRB_API bool mrbx_need_pathsep_p(const char *path, size_t len);
MRB_API mrbx_component_name mrbx_split_path(const char *path, size_t len);

MRB_END_DECL

#endif /* MRUBY_AUX_COMPONENT_NAME_H */
