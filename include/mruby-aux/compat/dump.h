#ifndef MRUBY_AUX_COMPAT_DUMP_H
#define MRUBY_AUX_COMPAT_DUMP_H 1

#include <mruby.h>
#include <mruby/dump.h>

#if MRUBY_RELEASE_NO < 20002 && !defined(mrb_true_p)
MRB_API mrb_irep *mrb_read_irep_buf(mrb_state *mrb, const void *buf, size_t bufsize);
#endif

#endif /* MRUBY_AUX_COMPAT_DUMP_H */
