#include "mrbx_path_defs.h"

MRB_API void
mrbx_pathinfo_parse(mrbx_pathinfo *pinfo)
{
  const char *path = pinfo->path;
  const char *end = path + pinfo->len;

  pinfo->basename = 0;
  path = mrbx_path_skip_root(path, end);
  pinfo->rootterm = pinfo->dirterm = pinfo->extname = pinfo->nameterm = path - pinfo->path;

  for (; path < end && *path != '\0'; path++) {
    SKIPDIRSEP(path, end);
    if (path >= end) {
      break;
    }

    pinfo->dirterm = pinfo->nameterm;
    pinfo->basename = path - pinfo->path;
    pinfo->extname = path - pinfo->path;

    const char *extname;
    path = mrbx_path_skip_name(path, end, &extname);
    pinfo->extname = extname - pinfo->path;
    pinfo->nameterm = path - pinfo->path;
  }
}
