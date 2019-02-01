#include <mruby-aux.h>
#include <mruby-aux/mobptr.h>
#include <stdio.h>
#include <stdlib.h>

static void
print_as_free(mrb_state *mrb, void *ptr)
{
    putchar((intptr_t)ptr);
}

static void
free_with(mrb_state *mrb, void *ptr)
{
    free(ptr);
}

int
main(int argc, char *argv[])
{
    mrb_state *mrb = mrb_open();
    if (mrb == NULL) { return 1; }

    mrb_value mob = mrbx_mob_create(mrb);

    enum { entries = 26 };
    for (int i = 0; i < entries; i ++) {
        mrbx_mob_push(mrb, mob, (void *)((intptr_t)'a' + i), print_as_free);
    }

    for (int i = 0; i < entries / 2; i ++) {
        void *p = (void *)((intptr_t)'a' + i * 2);
        mrbx_mob_free_f *dfree = mrbx_mob_pop(mrb, mob, p);
        dfree(mrb, p);
    }

    mrbx_mob_compact(mrb, mob);

    {
        /* ヒープ */

        enum { entries2 = 40 };
        void *ptr[entries2];

        for (int i = 0; i < entries2; i ++) {
            ptr[i] = malloc(64);
            mrbx_mob_push(mrb, mob, ptr[i], free_with);
        }

        for (int i = 0; i < entries2 / 2; i ++) {
            mrbx_mob_free(mrb, mob, ptr[i]);
        }

        for (int i = 0; i < entries2 / 2; i ++) {
            ptr[i] = malloc(64);
            mrbx_mob_push(mrb, mob, ptr[i], free_with);
        }
    }

    putchar(' ');

    for (int i = 0; i < entries / 2; i ++) {
        mrbx_mob_push(mrb, mob, (void *)((intptr_t)'a' + i * 2), print_as_free);
    }

    mrbx_mob_cleanup(mrb, mob);

    mrb_close(mrb);

    putchar('\n');

    return 0;
}
