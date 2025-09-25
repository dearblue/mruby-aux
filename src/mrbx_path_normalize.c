#include "mrbx_path_defs.h"

#define ISRELPATH()     (dirtop == path)
#define STACKSIZE       12
#define ISSTACKEMPTY()  (namestack.index <= 1)
#define ISSTACKFULL()   (namestack.index >= STACKSIZE)
#define POPSTACK()      do { mrb_assert(!ISSTACKEMPTY()); namestack.index--; } while (0)
#define PUSHSTACK(OFF)  \
  do { \
    if (ISSTACKFULL()) { \
      memmove(namestack.offset, namestack.offset + 1, (STACKSIZE - 1) * sizeof(char *)); \
      namestack.offset[STACKSIZE - 1] = (OFF); \
      again |= 1; \
    } else { \
      namestack.offset[namestack.index++] = (OFF); \
    } \
  } while (0)

MRB_API void
mrbx_path_normalize(char path[], const char **end)
{
  const char *endpoint;

  if (!end) {
    end = &endpoint;
    *end = path + strlen(path);
  } else if (!*end) {
    *end = path + strlen(path);
  }

  const char *end0 = *end;
  char *o = path;       // 書き込み位置
  const char *p = path; // 走査位置

#ifdef WITHIN_WINDOWS_CODE
  if (*end - p >= 2) {
    if (ISDRIVELETTER(p)) {
      p += 2;
      o = (char *)p;
    } else if (ISDIRSEP(p[0]) && ISDIRSEP(p[1])) {
      o[0] = o[1] = '/';
      p = o += 2;
      SKIPDIRSEP(p, *end);
      if (o == p) {
        NEXTDIRSEP(p, *end);
        o = (char *)p;
      } else {
        const char *p0 = p;
        NEXTDIRSEP(p, *end);
        memmove(o, p0, p - p0);
        o += p - p0;
      }
    }
  }
#endif

  if (ISDIRSEP(*p)) {
    p++;
    *o++ = '/';
    SKIPDIRSEP(p, *end);
  }

  char *const dirtop = o;
  for (int again = 3; again == 3; p = dirtop, *end = o) {
    again = 0;
    o = dirtop;

    struct {
      int index;
      char *offset[STACKSIZE];
    } namestack = { 1, { o } };

    while (p < *end) {
      const char *p0 = p;
      NEXTDIRSEP(p, *end);
      if (p - p0 == 1 && p0[0] == '.') {
        if (namestack.index < 2) {
          goto skipsep;
        } else {
          o = namestack.offset[namestack.index - 1];
          goto dirsep;
        }
      } else if (p - p0 == 2 && p0[0] == '.' && p0[1] == '.') {
        if (namestack.index < 2) {
          if ((again & 1) || ISRELPATH()) {
            again |= (1 & again) << 1;
            o[0] = o[1] = '.';
            o += 2;
            namestack.offset[0] = o;
            namestack.index = 1;
            goto dirsep;
          } else {
            goto skipsep;
          }
        } else {
          POPSTACK();
          o = namestack.offset[namestack.index - 1];
          if (o == dirtop) {
            goto skipsep;
          } else {
            goto dirsep;
          }
        }
      } else {
        if (o < p0) {
          memmove(o, p0, p - p0);
        }
        o += p - p0;
        PUSHSTACK(o);
      }

    dirsep:
      if (p >= *end) {
        break;
      }

      *o++ = '/';

    skipsep:
      SKIPDIRSEP(p, *end);
    }

    // パス最後の「/」を無視する
    o = namestack.offset[namestack.index - 1];
  }

  if (ISEMPTYPATH(path, o)) {
    *o++ = '.';
#ifdef WITHIN_WINDOWS_CODE
  } else if (o - path > 2 && o - dirtop == 0 && o[-1] == '/' && path[0] == '/' && path[1] == '/') {
    o--; // ホスト名直後のスラッシュを除去
#endif
  }

  if (o < end0) {
    *o = '\0';
  }

  *end = o;
}
