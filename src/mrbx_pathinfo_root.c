#include "mrbx_path_defs.h"

MRB_API const char *
mrbx_pathinfo_root(const mrbx_pathinfo *pinfo, size_t *len)
{
  *len = pinfo->rootterm;

  if (*len > 0) {
    return pinfo->path;
  } else {
    return NULL;
  }
}
