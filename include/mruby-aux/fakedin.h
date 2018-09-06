#ifndef MRUBY_AUX_FAKEDIN_H_
#define MRUBY_AUX_FAKEDIN_H_ 1

#include <mruby.h>
#include <stdint.h>

MRB_BEGIN_DECL

struct mrbx_fakedin
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

void mrbx_fakedin_setup(mrb_state *mrb, mrb_value owner, struct mrbx_fakedin *input, mrb_value stream);
mrb_int mrbx_fakedin_read(mrb_state *mrb, mrb_value owner, struct mrbx_fakedin *input, const char **buf, mrb_int size);
mrb_bool mrbx_fakedin_eof(struct mrbx_fakedin *input);
int64_t mrbx_fakedin_total_in(struct mrbx_fakedin *input);

MRB_END_DECL

#endif /* MRUBY_AUX_FAKEDIN_H_ */
