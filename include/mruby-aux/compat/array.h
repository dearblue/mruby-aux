#ifndef MRUBY_AUX_COMPAT_ARRAY_H
#define MRUBY_AUX_COMPAT_ARRAY_H 1

#include "../common.h"
#include <mruby.h>
#include <mruby/array.h>

/* ARY_EMBED_P は 1.4.0 で追加 */
#ifndef ARY_EMBED_P
# define ARY_EMBED_P(A) FALSE
#endif

/* ARY_EMBED_LEN は 1.4.0 で追加 */
#ifndef ARY_EMBED_LEN
# define ARY_EMBED_LEN(A) 0
#endif

/* ARY_EMBED_PTR は 1.4.0 で追加 */
#ifndef ARY_EMBED_PTR
# define ARY_EMBED_PTR(A) ((mrb_value *)NULL)
#endif

/* ARY_PTR は 1.4.0 で追加 */
#ifndef ARY_PTR
# define ARY_PTR(A)     ((const mrb_value*)(A)->ptr)
#endif

/* ARY_LEN は 1.4.0 で追加 */
#ifndef ARY_LEN
# define ARY_LEN(A)     ((A)->len)
#endif

#ifndef RARY_PTR
# define RARY_PTR(A)    ARY_PTR((A))
#endif

#ifndef RARY_LEN
# define RARY_LEN(A)    ARY_LEN((A))
#endif

#ifndef ARY_SET_LEN
# define ARY_SET_LEN(A, L) ((A)->len = (L))
#endif

#if MRUBY_RELEASE_NO < 20001
MRB_API mrb_value mrb_ary_splice(mrb_state *mrb, mrb_value self, mrb_int head, mrb_int len, mrb_value rpl);
#endif

#endif /* MRUBY_AUX_COMPAT_ARRAY_H */
