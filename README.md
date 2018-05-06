# mruby-aux - An auxiliary library for mruby

mruby ライブラリを作成する際の補助関数などを集めたライブラリです。


## HOW TO USAGE

### mrbx_scanhash

mruby のハッシュ値をキーワード引数として解析します。

mruby スクリプトから直接利用できるものではありません。

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


## Specification

  * Package name: mruby-aux
  * Version: 0.2
  * Product quality: PROTOTYPE
  * Author: [dearblue](https://github.com/dearblue)
  * Project page: <https://github.com/dearblue/mruby-aux>
  * Licensing: [Creative Commons Zero License (CC0 / Public Domain)](LICENSE)
  * Requirement language features:
      * generic selection (C11) or function overloading (C++)
      * variadic macros (`__VA_ARGS__`) (C99 or C++11)
      * compound literals (C99)
      * flexible array member (C99)
      * variable length arrays (C99)
  * Dependency external mrbgems: (NONE)
