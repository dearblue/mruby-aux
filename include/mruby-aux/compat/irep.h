#ifndef MRUBY_AUX_COMPAT_IREP_H
#define MRUBY_AUX_COMPAT_IREP_H 1

#include <mruby.h>
#include <mruby/irep.h>

#if MRUBY_RELEASE_NO < 20100
MRB_API mrb_value mrb_load_irep_buf(mrb_state *mrb, const void *buf, size_t bufsize);
MRB_API mrb_value mrb_load_irep_buf_cxt(mrb_state *mrb, const void *buf, size_t bufsize, mrbc_context *ctx);
#endif

#endif /* MRUBY_AUX_COMPAT_IREP_H */
