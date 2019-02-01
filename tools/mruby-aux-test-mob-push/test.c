#include <mruby-aux.h>
#include <mruby-aux/mobptr.h>

int
main(int argc, char *argv[])
{
    for (int i = 0; i < 20; i ++) {
        mrb_state *mrb = mrb_open();
        if (mrb == NULL) { return 1; }

        mrb_value mob = mrbx_mob_create(mrb);
        void *p = mrb_malloc(mrb, 512);

        mrbx_mob_push(mrb, mob, p, mrb_free);

        mrb_close(mrb);
    }

    return 0;
}
