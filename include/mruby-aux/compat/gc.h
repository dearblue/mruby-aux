#ifndef MRUBY_AUX_COMPAT_GC_H
#define MRUBY_AUX_COMPAT_GC_H

#include <mruby.h>
#include <mruby/gc.h>

#if MRUBY_RELEASE_NO < 10300
typedef void mrbx_objspace_each_objects_ret;
# define MRB_EACH_OBJ_OK
#else
typedef int mrbx_objspace_each_objects_ret;
#endif

#endif /* MRUBY_AUX_COMPAT_GC_H */
