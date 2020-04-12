# mruby-aux - An auxiliary C library for mruby

mruby ライブラリを作成する際の C の補助関数などを集めたライブラリです。

また、mruby-1.2 から mruby-2.0 までのいくつかの非互換性部分を補うマクロ・関数を提供します。


## HOW TO USAGE

あなたの `mrbgem.rake` に `add_dependency "mruby-aux", github: "dearblue/mruby-aux"` を記述して下さい。

  - `mrbgem.rake`

    ```ruby
    MRuby::Gem::Specification.new("mruby-XXX") do |s|
      .....

      add_dependency "mruby-aux", github: "dearblue/mruby-aux"
    end
    ```

`build_config.rb` で `gem github: "dearblue/mruby-aux"` する場合は、`conf.cc.include_paths"` にインクルードディレクトリを追加する必要があります。

  - `build_config.rb`

    ```ruby
    MRuby::Build.new("host") do |conf|
      .....

      mrbx_dir = File.join(MRUBY_ROOT, "build/mrbgems/mruby-aux/include")
      conf.cc.include_paths << mrbx_dir
      conf.cxx.include_paths << mrbx_dir

      gem github: "dearblue/mruby-aux"
    end
    ```

### `mrb_value()` (by include/mruby-aux.h)

`mrb_value()` マクロ関数は、与えられた値を `mrb_value` に変換します。

C++ の多重定義プロトタイプ宣言風に記述すると次のようになります:

  - `mrb_value mrb_value(mrb_value obj)`  
    引数 `obj` をそのまま返します。
  - `mrb_value mrb_value(struct RXXX *obj)`  
    オブジェクトポインタを `mrb_value` として返します。
  - `mrb_value mrb_value(mrb_int num)`  
    整数値を `mrb_value` として返します。
  - `mrb_value mrb_value(mrb_float num)`  
    浮動小数点数値を `mrb_value` として返します。
  - `mrb_value mrb_value(const char *str)`  
    文字列オブジェクトを生成して返します。  
    (※C の場合のみ) 直接リテラル文字列を与えた場合、`mrb_str_new_lit()` によって文字列オブジェクトが生成されます。

### `mrbx_symbol()` / `SYMBOL()` (by include/mruby-aux/common.h)

`mrbx_symbol()` マクロ関数は、与えられた値を `mrb_sym` 値に変換します。`SYMBOL()` マクロ関数は、`mrb_state *` として定義された `mrb` 変数を内部で置き換えて `mrbx_symbol()` を呼びます。

C++ の多重定義プロトタイプ宣言風に記述すると次のようになります:

  - `mrb_sym mrbx_symbol(mrb_state *mrb, mrb_value obj)` / `mrb_sym SYMBOL(mrb_value obj)`  
    `mrb_symbol()` マクロ関数によって `mrb_sym` 値に変換します。
  - `mrb_sym mrbx_symbol(mrb_state *mrb, mrb_sym sym)` / `mrb_sym SYMBOL(mrb_sym sym)`  
    引数 `sym` をそのまま返します。
  - `mrb_sym mrbx_symbol(mrb_state *mrb, const char *str)` / `mrb_sym SYMBOL(const char *str)`  
    `mrb_sym` 値に変換して返します。  
    (※C の場合のみ) 直接リテラル文字列を与えた場合、`mrb_intern_lit()` によってシンボルが生成されます。

### `MRBX_TUPLE()` (by include/mruby-aux/array.h)

`MRBX_TUPLE()` マクロ関数は、引数列を配列オブジェクトにします。

  - `mrb_value MRBX_TUPLE(mrb_value item1, ...)`

### `mrbx_scanhash()` / `MRBX_SCANHASH()` (by include/mruby-aux/scanhash.h)

mruby のハッシュ値をキーワード引数として解析します。

```c
/* scan target as hash object, or nil */
mrb_value user_hash_object = mrb_hash_new(mrb);

mrb_value a, b, c, d; /* receive to these variables */
MRBX_SCANHASH(mrb, user_hash_object, mrb_nil_value(),
        MRBX_SCANHASH_ARG("a", &a, mrb_nil_value()),
        MRBX_SCANHASH_ARG("b", &b, mrb_false_valse()),
        MRBX_SCANHASH_ARG("c", &c, mrb_str_new_cstr(mrb, "abcdefg")),
        MRBX_SCANHASH_ARG("d", &d, mrb_fixnum_value(5)));
```

### mob ポインタマネージャオブジェクト API (mob ポインタ API)

mruby の GC 対象外となる一般的なポインタを GC で回収・破棄させるための API です。

[`mruby-aux/mobptr.h`](include/mruby-aux/mobptr.h) をインクルードすることによって利用可能です。

```c
#include <mruby-aux/mobptr.h>


mrb_value mob = mrbx_mob_create(mrb);
void *ptr = mrb_malloc(mrb, 12345);
mrbx_mob_push(mrb, mob, ptr, mrb_free);

...
この区間で例外などの大域脱出が起きた場合、mob オブジェクトの破棄とともに ptr が開放される
...

mrbx_mob_free(mrb, mob, ptr); // 明示的に ptr を解放することも可能
mrbx_mob_cleanup(mrb, mob);   // mob オブジェクトに結び付けられたポインタすべてを解放する (しなくても GC の時に開放される)
```

他の関数、使い方については [`mruby-aux/mobptr.h`](include/mruby-aux/mobptr.h) を見て下さい。

実際に使ったテストコードについては以下を見て下さい。

  - [`mruby-aux-test-mob-create-only`](tools/mruby-aux-test-mob-create-only/test.c)
  - [`mruby-aux-test-mob-push`](tools/mruby-aux-test-mob-push/test.c)
  - [`mruby-aux-test-mob-push-pop`](tools/mruby-aux-test-mob-push-pop/test.c)
  - [`mruby-aux-test-mob-mix`](tools/mruby-aux-test-mob-mix/test.c)


## Specification

  * Package name: mruby-aux
  * Version: 0.8
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
