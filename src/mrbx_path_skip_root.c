#include "mrbx_path_defs.h"

MRB_API const char *
mrbx_path_skip_root(const char path[], const char *end)
{
#ifdef WITHIN_WINDOWS_CODE
  if (end - path >= 2) {
    if (isalpha((uint8_t)path[0]) && path[1] == ':') {
      path += 2;
    } else if (ISDIRSEP(path[0]) && ISDIRSEP(path[1])) {
      path += 2;

      // 連続したディレクトリ区切り文字を読み飛ばす
      for (; path < end && *path != '\0'; path++) {
        if (!ISDIRSEP(*path)) {
          break;
        }
      }

      // ホスト名
      for (; path < end && *path != '\0'; path++) {
        if (ISDIRSEP(*path)) {
          path++;
          break;
        }
      }

      return path;
    }
  }
#endif

  if (path < end && ISDIRSEP(path[0])) {
    path++;
  }

  return path;
}
