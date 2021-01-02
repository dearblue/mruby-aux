#include <mruby-aux.h>
#include <mruby-aux/mobptr.h>

int
main(int argc, char *argv[])
{
  for (int i = 0; i < 20; i++) {
    mrb_state *mrb = mrb_open();
    if (mrb == NULL) { return 1; }

    mrb_value mob = mrbx_mob_create(mrb);

    (void)mob;

    mrb_close(mrb);
  }

  return 0;
}
