#ifndef MRUBY_AUX_INTERNALS_EXTEND_SPACE_H
#define MRUBY_AUX_INTERNALS_EXTEND_SPACE_H 1

#include <mruby.h>
#include <stdlib.h>
#include <stddef.h>

#define EXTEND_SPACE(TRAITS)                                            \
  do {                                                                  \
    if ((TRAITS(POINTER) - TRAITS(BASE)) > TRAITS(MAX)) {               \
EXTEND_SPACE_ERROR:                                                     \
      mrb_raise(mrb, E_RANGE_ERROR, "reached extend limits");           \
    }                                                                   \
                                                                        \
    if (TRAITS(END) - TRAITS(POINTER) <= TRAITS(ROOM)) {                \
      if ((TRAITS(END) - TRAITS(POINTER)) > TRAITS(MAX) ||              \
          (TRAITS(END) - TRAITS(POINTER)) + TRAITS(ROOM) > TRAITS(MAX)) { \
        goto EXTEND_SPACE_ERROR;                                        \
      }                                                                 \
                                                                        \
      size_t idx = TRAITS(POINTER) - TRAITS(BASE);                      \
      size_t ext = TRAITS(EXTEND)(mrb, TRAITS(END) - TRAITS(BASE), TRAITS(ROOM)); \
                                                                        \
      TRAITS(BASE) = (TRAITS(TYPE) *)mrb_realloc(mrb, TRAITS(BASE), sizeof(TRAITS(TYPE)) * ext); \
      TRAITS(END) = TRAITS(BASE) + ext;                                 \
      TRAITS(POINTER) = TRAITS(BASE) + idx;                             \
    }                                                                   \
  } while (0)                                                           \

#endif /* MRUBY_AUX_INTERNALS_EXTEND_SPACE_H */
