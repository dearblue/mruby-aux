#ifndef MRUBY_AUX_INTERNALS_VM_COMMON_H
#define MRUBY_AUX_INTERNALS_VM_COMMON_H 1

#include <mruby.h>
#include <mruby-aux/version.h>

#if MRBX_MRUBY_RELEASE_NO > 30000
# define MRBX_CI_CINFO_NONE             0
# define MRBX_CI_CINFO_SKIP             1
# define MRBX_CI_CINFO_DIRECT           2
# define MRBX_CI_CINFO_RESUMED          3
# define MRBX_CI_CINFO_NONE_P(CI)       ((mrb_bool)((CI)->cci == MRBX_CI_CINFO_NONE))
# define MRBX_CI_CINFO_SKIP_P(CI)       ((mrb_bool)((CI)->cci == MRBX_CI_CINFO_SKIP))
# define MRBX_CI_CINFO_DIRECT_P(CI)     ((mrb_bool)((CI)->cci == MRBX_CI_CINFO_DIRECT))
# define MRBX_CI_CINFO_RESUMED_P(CI)    ((mrb_bool)((CI)->cci == MRBX_CI_CINFO_RESUMED))
# define MRBX_CI_SET_CINFO_NONE(CI)     do { (CI)->cci = MRBX_CI_CINFO_NONE; } while (0)
# define MRBX_CI_SET_CINFO_SKIP(CI)     do { (CI)->cci = MRBX_CI_CINFO_SKIP; } while (0)
# define MRBX_CI_SET_CINFO_DIRECT(CI)   do { (CI)->cci = MRBX_CI_CINFO_DIRECT; } while (0)
# define MRBX_CI_SET_CINFO_RESUMED(CI)  do { (CI)->cci = MRBX_CI_CINFO_RESUMED; } while (0)
#else
# define MRBX_CI_CINFO_NONE             0
# define MRBX_CI_CINFO_SKIP             -1
# define MRBX_CI_CINFO_DIRECT           -2
# define MRBX_CI_CINFO_RESUMED          -3
# define MRBX_CI_CINFO_NONE_P(CI)       ((mrb_bool)((CI)->acc >= 0))
# define MRBX_CI_CINFO_SKIP_P(CI)       ((mrb_bool)((CI)->acc == MRBX_CI_CINFO_SKIP))
# define MRBX_CI_CINFO_DIRECT_P(CI)     ((mrb_bool)((CI)->acc == MRBX_CI_CINFO_DIRECT))
# define MRBX_CI_CINFO_RESUMED_P(CI)    ((mrb_bool)((CI)->acc == MRBX_CI_CINFO_RESUMED))
# define MRBX_CI_SET_CINFO_NONE(CI)     do { (CI)->acc = MRBX_CI_CINFO_NONE; } while (0)
# define MRBX_CI_SET_CINFO_SKIP(CI)     do { (CI)->acc = MRBX_CI_CINFO_SKIP; } while (0)
# define MRBX_CI_SET_CINFO_DIRECT(CI)   do { (CI)->acc = MRBX_CI_CINFO_DIRECT; } while (0)
# define MRBX_CI_SET_CINFO_RESUMED(CI)  do { (CI)->acc = MRBX_CI_CINFO_RESUMED; } while (0)
#endif

#endif // MRUBY_AUX_INTERNALS_VM_COMMON_H
