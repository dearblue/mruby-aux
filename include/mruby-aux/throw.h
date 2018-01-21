#ifndef MRUBY_AUX_THROW_H__
#define MRUBY_AUX_THROW_H__ 1

#include <mruby.h>
#include <mruby/throw.h>

#define MRBX_ENTER(MRB)                                         \
    do {                                                        \
        mrb_state *_mrbx_mrb_ = (MRB);                          \
        struct mrb_jmpbuf *_mrbx_prev_jmp_ = (_mrbx_mrb_)->jmp; \
        struct mrb_jmpbuf _mrbx_c_jmp_;                         \
                                                                \
        MRB_TRY(&_mrbx_c_jmp_) {                                \
            (_mrbx_mrb_)->jmp = &_mrbx_c_jmp_;                  \
            do {                                                \

#define MRBX_INERROR()                                          \
            } while (0);                                        \
            (_mrbx_mrb_)->jmp = _mrbx_prev_jmp_;                \
        } MRB_CATCH(&_mrbx_c_jmp_) {                            \
            do {                                                \

#define MRBX_LEAVE()                                            \
            } while (0);                                        \
            (_mrbx_mrb_)->jmp = _mrbx_prev_jmp_;                \
            MRB_THROW(_mrbx_prev_jmp_);                         \
        } MRB_END_EXC(&_mrbx_c_jmp_);                           \
    } while (0)                                                 \

#endif /* MRUBY_AUX_THROW_H__ */
