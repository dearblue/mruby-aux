#include <mruby-aux/value.h>
#include <mruby/version.h>

MRB_API const char *
mrbx_typename(enum mrb_vtype tt)
{
  switch (tt) {
  case MRB_TT_FALSE:      return "MRB_TT_FALSE";
  case MRB_TT_TRUE:       return "MRB_TT_TRUE";
  case MRB_TT_FLOAT:      return "MRB_TT_FLOAT";
  case MRB_TT_FIXNUM:     return "MRB_TT_FIXNUM";
  case MRB_TT_SYMBOL:     return "MRB_TT_SYMBOL";
  case MRB_TT_UNDEF:      return "MRB_TT_UNDEF";
  case MRB_TT_CPTR:       return "MRB_TT_CPTR";
  case MRB_TT_FREE:       return "MRB_TT_FREE";
  case MRB_TT_OBJECT:     return "MRB_TT_OBJECT";
  case MRB_TT_CLASS:      return "MRB_TT_CLASS";
  case MRB_TT_MODULE:     return "MRB_TT_MODULE";
  case MRB_TT_ICLASS:     return "MRB_TT_ICLASS";
  case MRB_TT_SCLASS:     return "MRB_TT_SCLASS";
  case MRB_TT_PROC:       return "MRB_TT_PROC";
  case MRB_TT_ARRAY:      return "MRB_TT_ARRAY";
  case MRB_TT_HASH:       return "MRB_TT_HASH";
  case MRB_TT_STRING:     return "MRB_TT_STRING";
  case MRB_TT_RANGE:      return "MRB_TT_RANGE";
  case MRB_TT_EXCEPTION:  return "MRB_TT_EXCEPTION";
  case MRB_TT_FILE:       return "MRB_TT_FILE";
  case MRB_TT_ENV:        return "MRB_TT_ENV";
  case MRB_TT_DATA:       return "MRB_TT_DATA";
  case MRB_TT_FIBER:      return "MRB_TT_FIBER";
#if MRUBY_RELEASE_NO >= 10300
  case MRB_TT_ISTRUCT:    return "MRB_TT_ISTRUCT";
  case MRB_TT_BREAK:      return "MRB_TT_BREAK";
#endif
  default:                return NULL;
  }
}
