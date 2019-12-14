#ifndef MRUBY_AUX_COMPAT_ARRAY_H
#define MRUBY_AUX_COMPAT_ARRAY_H 1

#include "../common.h"
#include <mruby.h>
#include <mruby/array.h>

/* ARY_PTR は 1.4.0 で追加 */
#ifndef ARY_PTR
#   define ARY_PTR(A)   ((const mrb_value*)(A)->ptr)
#endif

/* ARY_LEN は 1.4.0 で追加 */
#ifndef ARY_LEN
#   define ARY_LEN(A)   ((A)->len)
#endif

#ifndef RARY_PTR
#   define RARY_PTR(A)  ARY_PTR((A))
#endif

#ifndef RARY_LEN
#   define RARY_LEN(A)  ARY_LEN((A))
#endif

#ifndef ARY_SET_LEN
#   define ARY_SET_LEN(A, L) ((A)->len = (L))
#endif

#endif /* MRUBY_AUX_COMPAT_ARRAY_H */
