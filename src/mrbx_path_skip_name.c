#include "mrbx_path_defs.h"

MRB_API const char *
mrbx_path_skip_name(const char path[], const char *end, const char **extname)
{
  *extname = NULL;

  for (; path < end && *path == '.'; path++)
    ;

  for (; path < end && *path != '\0'; path++) {
    if (*path == '.') {
      *extname = path;
    } else if (ISDIRSEP(*path)) {
      break;
    }
  }

  if (!*extname || path - *extname < 2) {
    *extname = path;
  }

  return path;
}
