#include "mrbx_path_defs.h"

MRB_API const char *
mrbx_pathinfo_extname(const mrbx_pathinfo *pinfo, size_t *len)
{
  *len = pinfo->nameterm - pinfo->extname;

  if (*len > 0) {
    return pinfo->path + pinfo->extname;
  } else {
    return "";
  }
}
