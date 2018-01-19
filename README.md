# mruby-aux-scanhash

mruby のハッシュ値をキーワード引数として解析する C のための補助ライブラリです。

mruby スクリプトから直接利用できるものではありません。


  * Product name: [mruby-aux-scanhash](https://github.com/dearblue/mruby-aux-scanhash)
  * Version: 0.1
  * Product quality: PROTOTYPE
  * Author: [dearblue](https://github.com/dearblue)
  * Project page: <https://github.com/dearblue/mruby-aux-scanhash>
  * Licensing: [Creative Commons Zero License (CC0 / Public Domain)](LICENSE)
  * Dependency external mrbgems: (NONE)
  * Bundled C libraries (git-submodule'd): (NONE)

# HOW TO USAGE

```c
/* scan target as hash object, or nil */
mrb_value user_hash_object = mrb_hash_new(mrb);

mrb_value a, b, c, d, e, f; /* receive to these variables */
MRBX_SCANHASH(mrb, user_hash_object, mrb_nil_value(),
        MRBX_SCANHASH_ARGS("a", &a, mrb_nil_value()),
        MRBX_SCANHASH_ARGS("b", &b, mrb_false_valse()),
        MRBX_SCANHASH_ARGS("c", &c, mrb_str_new_cstr(mrb, "abcdefg")),
        MRBX_SCANHASH_ARGS("d", &d, mrb_fixnum_value(5)));
```
