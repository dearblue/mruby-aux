#include "mrbx_path_defs.h"

MRB_API const char *
mrbx_path_skip_separator(const char path[], const char *end)
{
  SKIPDIRSEP(path, end);

  return path;
}
