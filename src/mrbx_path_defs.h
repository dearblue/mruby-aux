#ifndef MRUBY_AUX_PRIVATE_MRBX_PATH_DEFS_H
#define MRUBY_AUX_PRIVATE_MRBX_PATH_DEFS_H 1

#include <mruby-aux/pathinfo.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#if (defined(_WIN32) && !defined(__CYGWIN__)) || defined(MRUBY_AUX_DEBUG_FORCE_WITH_WINDOWS_CODE)
# define WITHIN_WINDOWS_CODE            1
# define CASE_PATH_SEPARATOR            '/': case '\\'
# define ISDIRSEP(CH)                   ((CH) == '/' || (CH) == '\\')
# define ISDRIVELETTER(PATH)            (ISALPHA((PATH)[0]) && (PATH)[1] == ':')
# define ISEMPTYPATH(PATH, OFFSET)      ((PATH) == (OFFSET) || ((OFFSET) - (PATH) == 2 && ISDRIVELETTER(PATH)))
#else
# define CASE_PATH_SEPARATOR            '/'
# define ISDIRSEP(CH)                   ((CH) == '/')
# define ISEMPTYPATH(PATH, OFFSET)      ((PATH) == (OFFSET))
#endif

#define SKIPDIRSEP(P, E)                for (; (P) < (E) && ISDIRSEP(*(P)); (P)++)
#define NEXTDIRSEP(P, E)                for (; (P) < (E) && !ISDIRSEP(*(P)); (P)++)
#define SKIPDOT(P, E)                   for (; (P) < (E) && (*(P)) == '.'; (P)++)

#endif // MRUBY_AUX_PRIVATE_MRBX_PATH_DEFS_H
