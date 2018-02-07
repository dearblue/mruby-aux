#include <mruby-aux/fakedin.h>
#include <mruby-aux/string/drop.h>
#include <mruby/variable.h>

void
mrbx_fakedin_setup(mrb_state *mrb, mrb_value owner, struct mrbx_fakedin *input, mrb_value stream)
{
    mrb_iv_set(mrb, owner, mrb_intern_cstr(mrb, "input stream@mruby-aux"), stream);

    input->stream = stream;
    input->off = 0;

    if (mrb_string_p(stream)) {
        input->string.buf = RSTRING(stream);
        input->selector = -1;
    } else {
        input->read.buf = NULL;
        input->selector = 0;
    }
}

static int
mrbx_fakedin_read_from_stream_read(mrb_state *mrb, mrb_value owner, struct mrbx_fakedin *input, mrb_int size)
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
mrbx_fakedin_read_from_stream(mrb_state *mrb, mrb_value owner, struct mrbx_fakedin *input, char **buf, mrb_int size)
{
    if (!input->read.buf) {
        input->read.off = 0;
        input->read.buf = RSTRING(mrb_str_buf_new(mrb, size));
        mrb_iv_set(mrb, owner, mrb_intern_cstr(mrb, "input buffer@mruby-aux"), mrb_obj_value(input->read.buf));
        RSTR_SET_LEN(input->read.buf, 0);
    }

    int ai = mrb_gc_arena_save(mrb);
    mrb_int len = RSTR_LEN(input->read.buf);

    if (input->read.off > len) {
        RSTR_SET_LEN(input->read.buf, 0);
        len = 0;
    }

    while (size > (len - input->read.off)) {
        if (mrbx_fakedin_read_from_stream_read(mrb, owner, input, size) != 0) {
            break;
        }

        mrb_gc_arena_restore(mrb, ai);
        len = RSTR_LEN(input->read.buf);
    }

    if (size > (len - input->read.off)) {
        size = len - input->read.off;
        if (size == 0) {
            mrb_iv_set(mrb, owner, mrb_intern_cstr(mrb, "input buffer@mruby-aux"), mrb_nil_value());

            input->off = -input->off;
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
mrbx_fakedin_read_from_string(mrb_state *mrb, mrb_value owner, struct mrbx_fakedin *input, char **buf, mrb_int size)
{
    const mrb_int len = RSTR_LEN(input->string.buf);

    if (size > (len - input->string.off)) {
        size = len - input->string.off;

        if (size == 0) {
            mrb_iv_set(mrb, owner, mrb_intern_cstr(mrb, "input buffer@mruby-aux"), mrb_nil_value());

            input->off = -input->off;
            *buf = NULL;

            return -1;
        }
    }

    *buf = RSTR_PTR(input->string.buf) + input->string.off;
    input->string.off += size;

    return size;
}

mrb_int
mrbx_fakedin_read(mrb_state *mrb, mrb_value owner, struct mrbx_fakedin *input, char **buf, mrb_int size)
{
    if (size < 0) { mrb_raise(mrb, E_RUNTIME_ERROR, "wrong fetch size"); }
    if (size == 0) { *buf = NULL; return 0; }
    if (input->off < 0) { *buf = NULL; return -1; }

    if (input->selector < 0) {
        return mrbx_fakedin_read_from_string(mrb, owner, input, buf, size);
    } else {
        return mrbx_fakedin_read_from_stream(mrb, owner, input, buf, size);
    }
}

mrb_bool
mrbx_fakedin_eof(struct mrbx_fakedin *input)
{
    return !!(input->off < 0);
}

int64_t
mrbx_fakedin_total_in(struct mrbx_fakedin *input)
{
    if (input->off < 0) {
        return -input->off;
    } else {
        return input->off;
    }
}
