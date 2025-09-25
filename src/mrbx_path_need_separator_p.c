#include "mrbx_path_defs.h"

MRB_API mrb_bool
mrbx_path_need_separator_p(const char path[], const char *end)
{
  if (path >= end) {
    return TRUE;
  }

  path = mrbx_path_skip_root(path, end);

  return !(path == end || ISDIRSEP(end[-1]));
}
