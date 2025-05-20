#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <mruby-aux/component-name.h>

//#define MRUBY_AUX_DEBUG_FORCE_WITH_WINDOWS_CODE

#if !defined(__CYGWIN__) && (defined(_WIN32) || defined(_WIN64)) || defined(MRUBY_AUX_DEBUG_FORCE_WITH_WINDOWS_CODE)
# define WITHIN_WINDOWS_CODE
# define CASE_PATH_SEPARATOR '/': case '\\'
#else
# define CASE_PATH_SEPARATOR '/'
#endif

MRB_API bool
mrbx_pathsep_p(int ch)
{
  switch (ch) {
  case CASE_PATH_SEPARATOR:
    return true;
  default:
    return false;
  }
}

static const char *
skip_root_component(const char *path, const uintptr_t end)
{
#ifdef WITHIN_WINDOWS_CODE
  if (end - (uintptr_t)path >= 2) {
    if (mrbx_pathsep_p(path[0]) && mrbx_pathsep_p(path[1])) {
      path += 2;

      /* 連続したパス区切り文字を読み飛ばす */
      for (; (uintptr_t)path < end; path++) {
        if (!mrbx_pathsep_p(*path)) {
          break;
        }
      }

      for (; (uintptr_t)path < end; path++) {
        if (*path == '\0') {
          break;
        } else if (mrbx_pathsep_p(*path)) {
          path++;
          break;
        }
      }

      return path;
    } else if (isalpha((uint8_t)path[0]) && path[1] == ':') {
      path += 2;
    }
  }
#endif

  for (; (uintptr_t)path < end; path++) {
    if (!mrbx_pathsep_p(path[0])) {
      break;
    }
  }

  return path;
}

static const char *
skip_separator(const char *path, uintptr_t end)
{
  for (; (uintptr_t)path < end; path++) {
    if (!mrbx_pathsep_p(*path)) {
      break;
    }
  }

  return path;
}

static const char *
skip_leading_dot(const char *path, const uintptr_t end)
{
  for (; (uintptr_t)path < end && *path == '.'; path++)
    ;

  return path;
}

static const char *
scan_component_name(const char *path, const uintptr_t end, mrbx_component_name *cp)
{
  for (; (uintptr_t)path < end; path++) {
    if (*path == '\0' || mrbx_pathsep_p(*path)) {
      break;
    } else if (*path == '.') {
      cp->extname = path - cp->path;
    }
  }

  if ((path - cp->path) - cp->extname < 2) {
    cp->extname = path - cp->path;
  }
  cp->nameterm = path - cp->path;

  return path;
}

MRB_API mrbx_component_name
mrbx_split_path(const char *path, uint16_t len)
{
  mrbx_component_name cp = { path, len };
  if (len > UINT16_MAX) {
    cp.len = 0;
    return cp;
  }

  const uintptr_t end = (uintptr_t)path + len;

  cp.basename = path - cp.path;
  path = skip_root_component(path, end);
  cp.rootterm = cp.dirterm = cp.extname = cp.nameterm = path - cp.path;

  for (; (uintptr_t)path < end && *path != '\0'; path++) {
    path = skip_separator(path, end);
    if (path == NULL) { break; }

    cp.dirterm = cp.nameterm;
    cp.basename = path - cp.path;
    cp.extname = path - cp.path;

    path = skip_leading_dot(path, end);
    path = scan_component_name(path, end, &cp);
    if (cp.extname == cp.basename) { cp.extname = path - cp.path; }
  }

  return cp;
}

MRB_API bool
mrbx_need_pathsep_p(const char *path, uint16_t len)
{
  if (len == 0) { return true; }

  const uintptr_t end = (uintptr_t)path + len;
  path = skip_root_component(path, end);

  return !((uintptr_t)path == end ||
           mrbx_pathsep_p(((const char *)end)[-1]));
}
