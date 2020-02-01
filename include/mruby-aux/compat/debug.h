#ifndef MRUBY_AUX_COMPAT_DEBUG_H
#define MRUBY_AUX_COMPAT_DEBUG_H

#include <mruby.h>
#include <mruby/irep.h>
#include <mruby/debug.h>

#if MRUBY_RELEASE_NO < 20001
# define mrb_debug_get_filename(mrb, irep, pc)  mrb_debug_get_filename(irep, pc)
# define mrb_debug_get_line(mrb, irep, pc)      mrb_debug_get_line(irep, pc)
#endif

#endif /* MRUBY_AUX_COMPAT_DEBUG_H */
