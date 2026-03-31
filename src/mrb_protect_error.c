#include <mruby-aux/version.h>

#if MRBX_MRUBY_RELEASE_NO <= 30000
#include <mruby.h>
#include <mruby-aux/compat/error.h>
#include <mruby-aux/vmext.h>
#include "vm-common.h"

struct mrb_protect_error_bridge
{
  mrb_protect_error_func *func;
  void *userdata;
};

static mrb_value
mrb_protect_error_bridge(mrb_state *mrb, mrb_value userdata)
{
  union { mrb_value v; struct mrb_protect_error_bridge *p; } wrapper = { userdata };
  struct mrb_protect_error_bridge *bridge = wrapper.p;
  return bridge->func(mrb, bridge->userdata);
}

MRB_API mrb_value
mrb_protect_error(mrb_state *mrb, mrb_protect_error_func *body, void *userdata, mrb_bool *error)
{
  struct mrb_protect_error_bridge bridge = { body, userdata };
  union { struct mrb_protect_error_bridge *p; mrb_value v; } wrapper = { &bridge };
  int ai = mrb_gc_arena_save(mrb);
  struct mrb_context *c = mrb->c;
  ptrdiff_t cioff = c->ci - c->cibase;
  mrb_value ret = mrb_protect(mrb, mrb_protect_error_bridge, wrapper.v, error);
  mrb_gc_arena_restore(mrb, ai);
  mrb_gc_protect(mrb, ret);

  if (mrb->c == c) {
    while (c->ci - c->cibase > cioff) {
      mrbx_vm_cipop(mrb);
    }
  } else {
    c = mrb->c;
    while (MRBX_CI_CINFO_DIRECT_P(c->ci) && c->ci > c->cibase) {
      mrbx_vm_cipop(mrb);
    }
  }

  return ret;
}
#endif // MRBX_MRUBY_RELEASE_NO <= 30000
