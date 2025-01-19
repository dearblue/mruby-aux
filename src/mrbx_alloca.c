#include <mruby.h>
#include <mruby-aux/utils.h>
#include <mruby/string.h>

MRB_API void *
mrbx_alloca(mrb_state *mrb, size_t size)
{
  if (size == 0) {
    return NULL;
  }

  struct RString *s = (struct RString *)mrb_obj_alloc(mrb, MRB_TT_STRING, NULL);
  s->as.heap.ptr = (char *)mrb_malloc(mrb, size);
  //s->as.heap.len = s->as.heap.aux.capa = size - 1;  // 整数型変換による桁落ちを考慮すると行う必要性がない
  //s->as.heap.ptr[size - 1] = '\0';                  // 必要であれば呼び出し元が責任を追う
  return s->as.heap.ptr;
}
