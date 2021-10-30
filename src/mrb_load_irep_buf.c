#include <mruby-aux/compat/irep.h>
#include <mruby-aux/dump.h>

#if MRUBY_RELEASE_NO < 20100
MRB_API mrb_value
mrb_load_irep_buf(mrb_state *mrb, const void *buf, size_t bufsize)
{
  if (!mrbx_taste_irep_buf(mrb, buf, bufsize, mrb_nil_value(), FALSE)) {
    return mrb_nil_value();
  } else {
    return mrb_load_irep(mrb, (const uint8_t *)buf);
  }
}
#endif
