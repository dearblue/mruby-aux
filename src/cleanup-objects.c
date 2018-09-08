#include <mruby.h>
#include <mruby/gc.h>
#include <mruby-aux/utils.h>

#if MRUBY_RELEASE_NO < 10300
typedef void mrb_objspace_each_objects_ret;
# define MRB_EACH_OBJ_OK
#else
typedef int mrb_objspace_each_objects_ret;
#endif

struct mrbx_cleanup_data_objects
{
    int num;
    const struct mrb_data_type **types;
};

static mrb_objspace_each_objects_ret
cleanup_objects(mrb_state *mrb, struct RBasic *o, void *p)
{
    struct RData *d = (struct RData *)o;
    const struct mrbx_cleanup_data_objects *args = (const struct mrbx_cleanup_data_objects *)p;
    int num = args->num;

    if (d->tt == MRB_TT_DATA) {
        for (int i = 0; i < num; i ++) {
            if (d->type == args->types[i]) {
                args->types[i]->dfree(mrb, d->data);
                d->data = NULL;
                break;
            }
        }
    }

    return MRB_EACH_OBJ_OK;
}

MRB_API void
mrbx_cleanup_data_objects(mrb_state *mrb, int num, const struct mrb_data_type *types[])
{
    const struct mrbx_cleanup_data_objects args = { num, types };

    mrb_objspace_each_objects(mrb, cleanup_objects, (void *)&args);
}
