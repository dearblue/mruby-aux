#include <mruby.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby-aux/object.h>
#include <stdbool.h>

typedef mrb_sym intern_opaque_f(mrb_state *mrb, void *name);

static struct RClass *
module_dig_common(mrb_state *mrb, struct RClass *start_module, int num, void **path, intern_opaque_f *intern, bool noraise)
{
  struct RClass *m = (start_module ? start_module : mrb->object_class);
  mrb_value v = mrb_obj_value(m);

  for (; num > 0; num--, path++) {
    v = mrb_const_get(mrb, v, intern(mrb, *path));
    switch (mrb_type(v)) {
    case MRB_TT_CLASS:
    case MRB_TT_MODULE:
    case MRB_TT_SCLASS:
      break;
    default:
      if (noraise) {
        return NULL;
      } else {
        mrb_raise(mrb, E_TYPE_ERROR, "not a class/module");
      }
    }
    m = mrb_class_ptr(v);
  }

  return m;
}

static struct RClass *
module_dig_by_cstr(mrb_state *mrb, struct RClass *start_module, int num, const char *const *path)
{
  return module_dig_common(mrb, start_module, num, (void **)path, (intern_opaque_f *)mrb_intern_cstr, true);
}

static intern_opaque_f intern_passthrough;

static mrb_sym
intern_passthrough(mrb_state *mrb, void *name)
{
  (void)mrb;
  return (mrb_sym)(uintptr_t)name;
}

static struct RClass *
module_dig_by_id(mrb_state *mrb, struct RClass *start_module, int num, const mrb_sym *path)
{
  return module_dig_common(mrb, start_module, num, (void **)path, intern_passthrough, true);
}

MRB_API mrb_bool
mrbx_obj_is_kind_of_checked(mrb_state *mrb, mrb_value obj, const char *name)
{
  return mrbx_obj_is_kind_of_checked_dig(mrb, obj, NULL, 1, &name);
}

MRB_API mrb_bool
mrbx_obj_is_kind_of_checked_dig(mrb_state *mrb, mrb_value obj, struct RClass *start_module, int num, const char *const *path)
{
  struct RClass *m = module_dig_by_cstr(mrb, start_module, num, path);
  return (m ? mrb_obj_is_kind_of(mrb, obj, m) : FALSE);
}

MRB_API mrb_bool
mrbx_obj_is_kind_of_checked_id(mrb_state *mrb, mrb_value obj, mrb_sym name)
{
  return mrbx_obj_is_kind_of_checked_dig_id(mrb, obj, NULL, 1, &name);
}

MRB_API mrb_bool
mrbx_obj_is_kind_of_checked_dig_id(mrb_state *mrb, mrb_value obj, struct RClass *start_module, int num, const mrb_sym *path)
{
  struct RClass *m = module_dig_by_id(mrb, start_module, num, path);
  return (m ? mrb_obj_is_kind_of(mrb, obj, m) : FALSE);
}
