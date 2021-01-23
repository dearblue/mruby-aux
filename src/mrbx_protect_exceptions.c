#include <mruby-aux/error.h>
#include <mruby/error.h>

struct protect_exc_try
{
  mrb_func_t body;
  const mrb_value *b_data;
  mrb_func_t rescue;
  const mrb_value *r_data;
  mrb_int len;
  struct RClass **classes;
};

static mrb_value
protect_exc_try(mrb_state *mrb, mrb_value arg)
{
  struct protect_exc_try *p = (struct protect_exc_try *)mrb_cptr(arg);

  return mrb_rescue_exceptions(mrb, p->body, *p->b_data, p->rescue, *p->r_data, p->len, p->classes);
}

MRB_API mrb_value
mrbx_protect_exceptions(
    mrb_state *mrb,
    mrb_func_t body, mrb_value b_data,
    mrb_func_t rescue, mrb_value r_data,
    mrb_func_t ensure, mrb_value e_data,
    mrb_int len, struct RClass *classes[])
{
  if (ensure) {
    if (rescue) {
      struct protect_exc_try arg = { body, &b_data, rescue, &r_data, len, classes };

      return mrb_ensure(mrb, protect_exc_try, mrb_cptr_value(mrb, (void *)&arg), ensure, e_data);
    } else {
      return mrb_ensure(mrb, body, b_data, ensure, e_data);
    }
  } else if (rescue) {
    return mrb_rescue_exceptions(mrb, body, b_data, rescue, r_data, len, classes);
  } else {
    return body(mrb, b_data);
  }
}
