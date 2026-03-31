#include "mrbx_path_defs.h"

MRB_API const char *
mrbx_pathinfo_dirname(const mrbx_pathinfo *pinfo, size_t *len)
{
  *len = pinfo->dirterm;

  if (*len > 0) {
    return pinfo->path;
  } else {
    *len = 1;
    return ".";
  }
}
