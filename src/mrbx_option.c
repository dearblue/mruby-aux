#include <mruby-aux/option.h>
#include <mruby-aux/utils.h>
#include <stdarg.h>
#include <string.h>

static mrbx_option_vprintf_func option_vprintf;

#ifdef MRB_DISABLE_STDIO
static int
option_vprintf(void *user, const char *fmt, va_list va)
{
  (void)user;
  (void)fmt;
  (void)va;

  return -1;
}
#else
static int
option_vprintf(void *user, const char *fmt, va_list va)
{
  (void)user;

  return vprintf(fmt, va);
}
#endif

static int
option_printf(mrbx_option_vprintf_func *print, void *user, const char *fmt, ...)
{
  int result;
  va_list va;

  va_start(va, fmt);
  result = print(user, fmt, va);
  va_end(va);

  return result;
}

static void
print_complement_endl(mrbx_option_vprintf_func *print, void *user, const char *str)
{
  if (str == NULL || str[0] == '\0' || str[strlen(str) - 1] != '\n') {
    option_printf(print, user, "\n");
  }
}

#define option_printf_unless_dry(estimation, print, user, ...)          \
  do {                                                                  \
    if ((estimation) > 0) {                                             \
      option_printf(print, user, __VA_ARGS__);                          \
    }                                                                   \
  } while (0)                                                           \

static int
option_print_descriptions(const mrbx_option *opt, mrbx_option_vprintf_func *print, void *user, int estimation, int indent)
{
  if (indent < 0) {
    indent = 0;
  }

  int maxwidth = (estimation > 0 ? estimation : 0);

  FOREACH_NLIST(const mrbx_option_entry, *e, opt->optnum, opt->entries) {
    int width = 0;

    if (e->opt < 0) {
      // 隠しオプション
      continue;
    }

    if (e->opt == '\0' && e->optlong == NULL) {
      if (estimation > 0) {
        if (e->summary) {
          if (e->arg) {
            option_printf(print, user, "%*s", indent + estimation + 2, "");
          }

          option_printf(print, user, "%s", e->summary);
        }

        print_complement_endl(print, user, e->summary);
      }

      continue;
    }

    option_printf_unless_dry(estimation, print, user, "%*s", indent, "");

    if (ISPRINT(e->opt)) {
      option_printf_unless_dry(estimation, print, user, "-%c", e->opt);
      width += 2;

      if (e->optlong) {
        option_printf_unless_dry(estimation, print, user, ", ");
        width += 2;
      } else if (e->arg) {
        option_printf_unless_dry(estimation, print, user, "%s", e->arg);
        width += strlen(e->arg);
      }
    }

    if (e->optlong) {
      option_printf_unless_dry(estimation, print, user, "--%s", e->optlong);
      width += 2 + strlen(e->optlong);

      if (e->arg) {
        option_printf_unless_dry(estimation, print, user, "=%s", e->arg);
        width += 1 + strlen(e->arg);
      }
    }

    if (estimation > 0) {
      if (e->summary) {
        if (maxwidth < width) {
          option_printf(print, user, "\n  %*s", maxwidth, "");
        } else {
          option_printf(print, user, "%*s", maxwidth - width, "");
        }

        option_printf(print, user, "  %s", e->summary);
        print_complement_endl(print, user, e->summary);
      } else {
        option_printf(print, user, "\n");
      }
    } else if (maxwidth < width) {
      maxwidth = width;
    }
  }

  return maxwidth;
}

MRB_API void
mrbx_option_usage(const mrbx_option *opt, int width, int indent, const mrbx_option_vprintf *print, const char *header, const char *footer, ...)
{
  va_list va;
  mrbx_option_vprintf_func *print_func = (print && print->vprintf_func ? print->vprintf_func : option_vprintf);
  void *user = (print ? print->user : NULL);

  if (header) {
    va_start(va, footer);

    if (header) {
      print_func(user, header, va);
    }

    print_complement_endl(print_func, user, header);

    if (!footer) {
      va_end(va);
    }
  } else {
    option_printf(print_func, user, "Usage: %s [switch...] file...\n", opt->progname);
  }

  if (width < 1) {
    width = option_print_descriptions(opt, print_func, user, 0, indent);
  }

  option_print_descriptions(opt, print_func, user, width, indent);

  if (footer) {
    if (!header) {
      va_start(va, footer);
    }

    if (footer) {
      print_func(user, footer, va);
    }

    print_complement_endl(print_func, user, footer);
    va_end(va);
  }
}

static int
option_parse_long(mrbx_option *o)
{
  int optlen;
  const char *term = strchr(o->opt, '=');

  if (!term) {
    optlen = strlen(o->opt);
    term = o->opt + optlen;
  } else {
    optlen = term - o->opt;
  }

  FOREACH_NLIST(const mrbx_option_entry, *e, o->optnum, o->entries) {
    if (e->opt != '\0' && e->optlong &&
        (int)strlen(e->optlong) == optlen && memcmp(o->opt, e->optlong, optlen) == 0) {
      if (e->arg) {
        if (term) {
          o->arg = term + 1;
        } else if (o->argc > 1) {
          o->arg = *o->argv++;
          o->argc--;
        } else {
          o->arg = o->opt + optlen;
        }
      } else {
        // term != NULL であれば捨てる
        o->arg = NULL;
      }

      o->opt = NULL;

      return e->opt > 0 ? e->opt : -e->opt;
    }
  }

  o->arg = NULL;

  return MRBX_OPTION_UNKNOWN_LONG;
}

static int
option_parse(mrbx_option *o)
{
  FOREACH_NLIST(const mrbx_option_entry, *e, o->optnum, o->entries) {
    int t = *o->opt;

    if (t == e->opt || t == -e->opt) {
      o->opt++;

      if (e->arg) {
        if (*o->opt != '\0') {
          o->arg = o->opt;
        } else if (o->argc > 0) {
          o->arg = *o->argv++;
          o->argc--;
        } else {
          o->arg = o->opt;
        }
        o->opt = NULL;
      }

      return t;
    }
  }

  o->arg = NULL;

  return MRBX_OPTION_UNKNOWN;
}

#define OPTION_COMPLETED(RET)                                           \
  do {                                                                  \
    o->opt = NULL;                                                      \
    o->arg = NULL;                                                      \
    return (RET);                                                       \
  } while (0)                                                           \

MRB_API int
mrbx_option_parse(mrbx_option *o)
{
  if (o->argc <= 0 && (!o->opt || *o->opt == '\0')) {
    OPTION_COMPLETED(MRBX_OPTION_END);
  }

  if (!o->opt || *o->opt == '\0') {
    o->opt = *o->argv;

    if (o->opt == NULL || o->opt[0] != '-' || o->opt[1] == '\0') {
      OPTION_COMPLETED(MRBX_OPTION_BREAK);
    }

    o->opt++;
    o->argv++;
    o->argc--;

    if (*o->opt == '-') {
      o->opt++;

      if (*o->opt == '\0') {
        OPTION_COMPLETED(MRBX_OPTION_STOP);
      }

      return option_parse_long(o);
    }
  }

  return option_parse(o);
}
