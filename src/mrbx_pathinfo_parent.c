#include "mrbx_path_defs.h"

MRB_API const char *
mrbx_pathinfo_parent(const mrbx_pathinfo *pinfo, size_t *len)
{
  if (pinfo->rootterm > 0 && pinfo->rootterm == pinfo->nameterm) {
    *len = pinfo->rootterm;
    return pinfo->path;
  } else if (pinfo->dirterm == 0) {
    *len = 1;
    return ".";
  } else {
    *len = pinfo->dirterm;
    return pinfo->path;
  }
}
