#include "mrbx_path_defs.h"

MRB_API const char *
mrbx_pathinfo_basename(const mrbx_pathinfo *pinfo, size_t *len)
{
  *len = pinfo->nameterm - pinfo->basename;

  if (*len > 0) {
    return pinfo->path + pinfo->basename;
  } else {
    *len = 1;
    return ".";
  }
}
