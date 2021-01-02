#include <mruby-aux/fakedin.h>
#include <mruby-aux/string.h>
#include <mruby-aux/utils.h>
#include <mruby/variable.h>
#include <mruby/data.h>

#define id_iv_stream_private mrb_intern_lit(mrb, "input stream@mruby-aux")
#define id_iv_buffer_private mrb_intern_lit(mrb, "input buffer@mruby-aux")

struct fakedin
{
  /* 読み込み対象 */
  mrb_value stream;

  union {
    struct {
      /* 現在の読み込んだ総バイト量 */
      int64_t off;

      int64_t selector;
    };

    struct {
      int64_t off;

      const int64_t constant_negative;

      /*
       * stream の実体
       */
      struct RString *buf;
    } string;

    struct {
      const int64_t total;

      /* buf 中におけるバイト位置 */
      int64_t off;

      /*
       * stream を段階的に読み込むデータ領域
       */
      struct RString *buf;
    } read;
  };
};

static inline mrb_bool
fakedin_string_p(const struct fakedin *p)
{
  return (mrb_bool)(p->selector < 0 ? TRUE : FALSE);
}

static const mrb_data_type fakedin_type = { "faked-in@mruby-aux", mrb_free };

MRB_API mrb_value
mrbx_fakedin_new(mrb_state *mrb, mrb_value stream)
{
  struct RData *d = mrb_data_object_alloc(mrb, mrb->object_class, NULL, &fakedin_type);
  struct fakedin *input = (struct fakedin *)mrb_calloc(mrb, 1, sizeof(struct fakedin));
  mrb_value fake = mrb_obj_value(d);

  d->data = input;
  input->stream = stream;
  input->off = 0;

  mrb_iv_set(mrb, fake, id_iv_stream_private, stream);

  if (mrb_string_p(stream)) {
    input->string.buf = RSTRING(stream);
    input->selector = -1;
  } else {
    input->read.buf = NULL;
    input->selector = 0;
  }

  return fake;
}

static struct fakedin *
get_fakedin(mrb_state *mrb, mrb_value o)
{
  return (struct fakedin *)mrbx_getref(mrb, o, &fakedin_type);
}

static int
mrbx_fakedin_read_from_stream_read(mrb_state *mrb, struct fakedin *input, mrb_int size)
{
  mrb_int len = RSTR_LEN(input->read.buf);

  if (input->read.off == len) {
    const mrb_value args[] = { mrb_fixnum_value(size), mrb_obj_value(input->read.buf) };
    mrb_value s = mrb_funcall_argv(mrb, input->stream, mrb_intern_lit(mrb, "read"), 2, args);

    if (mrb_nil_p(s)) { return -1; }

    if (!mrb_obj_eq(mrb, s, mrb_obj_value(input->read.buf))) {
      mrb_raisef(mrb, E_TYPE_ERROR,
                 "object mismatch from %S.read",
                 input->stream);
    }

    mrb_str_modify(mrb, input->read.buf);
  } else {
    const mrb_value args[] = { mrb_fixnum_value(size - len) };
    mrb_value s = mrb_funcall_argv(mrb, input->stream, mrb_intern_lit(mrb, "read"), 1, args);

    if (mrb_nil_p(s)) { return -1; }

    mrb_check_type(mrb, s, MRB_TT_STRING);

    mrbx_str_drop(mrb, input->read.buf, 0, input->read.off);
    mrb_str_concat(mrb, mrb_obj_value(input->read.buf), s);
  }

  input->read.off = 0;

  return 0;
}

static mrb_int
mrbx_fakedin_read_from_stream(mrb_state *mrb, mrb_value fake, struct fakedin *input, const char **buf, mrb_int size)
{
  if (!input->read.buf) {
    mrb_value bufobj = mrb_str_buf_new(mrb, size);
    input->read.off = 0;
    input->read.buf = RSTRING(bufobj);
    RSTR_SET_LEN(input->read.buf, 0);
    mrb_iv_set(mrb, fake, id_iv_buffer_private, mrb_obj_value(input->read.buf));
  }

  int ai = mrb_gc_arena_save(mrb);
  mrb_int len = RSTR_LEN(input->read.buf);

  if (input->read.off > len) {
    RSTR_SET_LEN(input->read.buf, 0);
    len = 0;
  }

  while (size > (len - input->read.off)) {
    if (mrbx_fakedin_read_from_stream_read(mrb, input, size) != 0) {
      break;
    }

    mrb_gc_arena_restore(mrb, ai);
    len = RSTR_LEN(input->read.buf);
  }

  if (size > (len - input->read.off)) {
    size = len - input->read.off;
    if (size == 0) {
      input->off = ~input->off;
      *buf = NULL;

      return -1;
    }
  }

  *buf = RSTR_PTR(input->read.buf) + input->read.off;
  input->read.off += size;
  input->off += size;

  return size;
}

static mrb_int
mrbx_fakedin_read_from_string(mrb_state *mrb, mrb_value fake, struct fakedin *input, const char **buf, mrb_int size)
{
  const mrb_int len = RSTR_LEN(input->string.buf);

  if (size > (len - input->string.off)) {
    size = len - input->string.off;

    if (size == 0) {
      input->off = ~input->off;
      *buf = NULL;

      return -1;
    }
  }

  *buf = RSTR_PTR(input->string.buf) + input->string.off;
  input->string.off += size;

  return size;
}

MRB_API mrb_int
mrbx_fakedin_read(mrb_state *mrb, mrb_value fakedin, const char **buf, mrb_int size)
{
  if (size < 0) { mrb_raise(mrb, E_RUNTIME_ERROR, "wrong fetch size"); }
  if (size == 0) { *buf = NULL; return 0; }

  struct fakedin *input = get_fakedin(mrb, fakedin);

  if (input->off < 0) { *buf = NULL; return -1; }

  if (fakedin_string_p(input)) {
    return mrbx_fakedin_read_from_string(mrb, fakedin, input, buf, size);
  } else {
    return mrbx_fakedin_read_from_stream(mrb, fakedin, input, buf, size);
  }
}

MRB_API mrb_bool
mrbx_fakedin_eof(mrb_state *mrb, mrb_value fakedin)
{
  struct fakedin *input = get_fakedin(mrb, fakedin);

  return !!(input->off < 0);
}

MRB_API int64_t
mrbx_fakedin_total_in(mrb_state *mrb, mrb_value fakedin)
{
  struct fakedin *input = get_fakedin(mrb, fakedin);

  if (input->off < 0) {
    return ~input->off;
  } else {
    return input->off;
  }
}

MRB_API mrb_value
mrbx_fakedin_stream(mrb_state *mrb, mrb_value fakedin)
{
  struct fakedin *input = get_fakedin(mrb, fakedin);

  return input->stream;
}
