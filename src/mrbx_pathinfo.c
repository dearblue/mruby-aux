#include <mruby-aux/pathinfo.h>
#include <ctype.h>
#include <stdlib.h>

//#define MRUBY_AUX_DEBUG_FORCE_WITH_WINDOWS_CODE

#if !defined(__CYGWIN__) && (defined(_WIN32) || defined(_WIN64)) || defined(MRUBY_AUX_DEBUG_FORCE_WITH_WINDOWS_CODE)
# define WITHIN_WINDOWS_CODE 1
# define CASE_PATH_SEPARATOR '/': case '\\'
#else
# define CASE_PATH_SEPARATOR '/'
#endif

MRB_API mrb_bool
mrbx_pathsep_p(int ch)
{
  switch (ch) {
  case CASE_PATH_SEPARATOR:
    return TRUE;
  default:
    return FALSE;
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

  if ((uintptr_t)path < end && mrbx_pathsep_p(path[0])) {
    path++;
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
scan_pathinfo(const char *path, const uintptr_t end, mrbx_pathinfo *pi)
{
  for (; (uintptr_t)path < end; path++) {
    if (*path == '\0' || mrbx_pathsep_p(*path)) {
      break;
    } else if (*path == '.') {
      pi->extname = path - pi->path;
    }
  }

  if ((path - pi->path) - pi->extname < 2) {
    pi->extname = path - pi->path;
  }
  pi->nameterm = path - pi->path;

  return path;
}

MRB_API mrbx_pathinfo
mrbx_split_path(const char *path, uint16_t len)
{
  mrbx_pathinfo pi = { path, len };
  if (len > UINT16_MAX) {
    pi.len = 0;
    return pi;
  }

  const uintptr_t end = (uintptr_t)path + len;

  pi.basename = path - pi.path;
  path = skip_root_component(path, end);
  pi.rootterm = pi.dirterm = pi.extname = pi.nameterm = path - pi.path;

  for (; (uintptr_t)path < end && *path != '\0'; path++) {
    path = skip_separator(path, end);
    if ((uintptr_t)path >= end) { break; }

    pi.dirterm = pi.nameterm;
    pi.basename = path - pi.path;
    pi.extname = path - pi.path;

    path = skip_leading_dot(path, end);
    path = scan_pathinfo(path, end, &pi);
    if (pi.extname == pi.basename) { pi.extname = path - pi.path; }
  }

  return pi;
}

MRB_API mrb_bool
mrbx_need_pathsep_p(const char *path, uint16_t len)
{
  if (len == 0) { return TRUE; }

  const uintptr_t end = (uintptr_t)path + len;
  path = skip_root_component(path, end);

  return !((uintptr_t)path == end || mrbx_pathsep_p(((const char *)end)[-1]));
}
