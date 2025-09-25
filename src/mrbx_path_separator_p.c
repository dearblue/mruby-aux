#include "mrbx_path_defs.h"

MRB_API mrb_bool
mrbx_path_separator_p(int32_t ch)
{
  return ISDIRSEP(ch);
}
