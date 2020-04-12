#include <mruby-aux/utils.h>
#include <mruby/error.h>

#ifdef MRB_WORD_BOXING
static mrb_value
cptr_value_trial(mrb_state *mrb, mrb_value wrapper)
{
  void *ptr = *(void **)&wrapper;
  return mrb_cptr_value(mrb, ptr);
}

MRB_API mrb_value
mrbx_cptr_value(mrb_state *mrb, void *ptr)
{
  mrb_bool err;
  mrb_value wrapper;
  *(void **)&wrapper = ptr;
  mrb_value ret = mrb_protect(mrb, cptr_value_trial, wrapper, &err);

  if (err) {
    return mrb_nil_value();
  } else {
    return ret;
  }
}
#else
MRB_API mrb_value
mrbx_cptr_value(mrb_state *mrb, void *ptr)
{
  return mrb_cptr_value(mrb, ptr);
}
#endif
