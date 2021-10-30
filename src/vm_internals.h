#ifndef MRUBY_AUX_VM_INTERNALS_H
#define MRUBY_AUX_VM_INTERNALS_H 1

#include <mruby-aux/common.h>
#include <mruby-aux/version.h>

#if MRBX_MRUBY_RELEASE_NO > 30000
# define ARGS_FORCE_PACK_THRESHOLD 14
# define ARGC_PACKED 15
# define CI_ARGC(CI) (CI)->n
# define CI_ARGS_PACKED_P(CI) (CI_ARGC(CI) == ARGC_PACKED)
# define CI_ARGS_KW_SPACE(CI) ((CI)->nk < ARGC_PACKED ? (CI)->nk * 2 : 1)
#else
# define ARGS_FORCE_PACK_THRESHOLD 30
# define ARGC_PACKED -1
# define CI_ARGC(CI) (CI)->argc
# define CI_ARGS_PACKED_P(CI) (CI_ARGC(CI) < 0)
# define CI_ARGS_KW_SPACE(CI) ((void)(CI), 0)
#endif // MRBX_MRUBY_RELEASE_NO

#define CI_ARGC_SET(CI, ARGC) do { CI_ARGC(CI) = (ARGC); } while (0)
#define CI_ARGC_SET_PACKED(CI) CI_ARGC_SET(CI, ARGC_PACKED)

#endif /* MRUBY_AUX_VM_INTERNALS_H */
