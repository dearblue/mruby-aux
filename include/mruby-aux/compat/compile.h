#ifndef MRUBY_AUX_COMPAT_COMPILE_H
#define MRUBY_AUX_COMPAT_COMPILE_H

#include <mruby.h>
#include <mruby/compile.h>

#if MRUBY_RELEASE_NO < 20001
# define mrb_parser_get_filename(parser, idx) \
  mrb_intern_cstr((parser)->mrb, mrb_parser_get_filename(parser, idx))
#endif

#endif /* MRUBY_AUX_COMPAT_COMPILE_H */
