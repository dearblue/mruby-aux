#include <mruby-aux/pathinfo.h>
#include <ctype.h>
#include <stdlib.h>

//#define MRUBY_AUX_DEBUG_FORCE_WITH_WINDOWS_CODE

#if !defined(__CYGWIN__) && (defined(_WIN32) || defined(_WIN64)) || defined(MRUBY_AUX_DEBUG_FORCE_WITH_WINDOWS_CODE)
# define WITHIN_WINDOWS_CODE 1
# define CASE_PATH_SEPARATOR '/': case '\\'
# define ISDIRSEP(CH) ((CH) == '/' || (CH) == '\\')
#else
# define CASE_PATH_SEPARATOR '/'
# define ISDIRSEP(CH) ((CH) == '/')
#endif

#define SKIPDIRSEP(P, E)        for (; ISDIRSEP(*(P)); (P)++)
#define NEXTDIRSEP(P, E)        for (; (P) < (E) && !ISDIRSEP(*(P)); (P)++)
#define SKIPDOT(P, E)           for (; (P) < (E) && (*(P)) == '.'; (P)++)

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

MRB_API mrb_bool
mrbx_path_separator_p(int32_t ch)
{
  return ISDIRSEP(ch);
}

MRB_API mrb_bool
mrbx_path_need_separator_p(const char path[], const char *end)
{
  if (path >= end) {
    return TRUE;
  }

  path = mrbx_path_skip_root(path, end);

  return !(path == end || ISDIRSEP(end[-1]));
}

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

MRB_API const char *
mrbx_path_skip_separator(const char path[], const char *end)
{
  SKIPDIRSEP(path, end);

  return path;
}

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
