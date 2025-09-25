def assert_path_normalize(expect, input)
  assert_equal expect, AuxTest.path_normalize(input)
end

if AuxTest.windows?
  def CHOOSE(posix, windows)
    windows
  end
else
  def CHOOSE(posix, windows)
    posix
  end
end

assert "mrbx_path_normalize" do
  assert_path_normalize "a/b/c", "a/b/c"
  assert_path_normalize "a/b/c", "a/b/./c"
  assert_path_normalize ".", "a/.."
  assert_path_normalize "..", ".."
  assert_path_normalize "../../../../../../a/b/c/d/e/f/g/h", "././././././././././../../../../../../a/b/c/d/e/f/g/h"
  assert_path_normalize "a/b/c", "a/b/c/d/.."
  assert_path_normalize "a/b/c/D", "a/b/c/d/e/f/../../../D"
  assert_path_normalize "a/b/c/d/e/f/g/h/i/j/k/l/m/n", "a/b/c/d/e/f/g/h/i/j/k/l/m/n"
  assert_path_normalize "a/b/c/d/e/f/g/h/i/j/k/l/m", "a/b/c/d/e/f/g/h/i/j/k/l/m/n/.."
  assert_path_normalize "a/b/c/d", "a/b/c/d/e/f/g/h/i/j/k/l/m/n/../../../../../../../../../.."
  assert_path_normalize "../../../../../../../../../../../../../../../d/e/f/g/h/i/j/k",
                        "a/b/c/../../../../../../../../../../../../../../../../../../d/e/f/g/h/i/j/k"
  assert_path_normalize "../../../../../../../../../../../../../../../../../../d/e",
                        "../../../a/b/././././././c/../../../../../../../../../../../../../../../../../../d/e"
  assert_path_normalize "/", "/"
  assert_path_normalize CHOOSE("/g/h/i/j/k", "//a/g/h/i/j/k"),
                        "//a//.//.//.//.//.//.//.//.//.//b//c//.//.//.//.//.//.//.//.//.//.//.//.//d//e//f//..//..//..//..//..//..//..//..//..//..//..//..//..//..//.//.//g//h//i//j//k/"
  assert_path_normalize CHOOSE("/g", "//./g"),
                        "//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//.//a//b//..//..//..//..//..//c//d//..//..//..//..//..//..//..//..//..//..//..//..//..//..//..//e//f//..//..//..//..//..//..//..//..//..//..//..//..//g/"
  assert_path_normalize CHOOSE("a:", "a:/"), "a:/b/c/d/../../.."
  assert_path_normalize CHOOSE(".",  "a:/"), "a:/b/c/d/../../../.."
end

assert "mrbx_path_need_separator_p" do
  assert_equal true, AuxTest.path_need_separator?("a")
  assert_equal true, AuxTest.path_need_separator?("a.b")
  assert_equal true, AuxTest.path_need_separator?("a/b.c")
  assert_equal false, AuxTest.path_need_separator?("a/")
  assert_equal false, AuxTest.path_need_separator?("/")
end

assert "mrbx_path_separator_p" do
  assert_equal true, AuxTest.path_separator?("/")
  assert_equal false, AuxTest.path_separator?("a")
end

assert "mrbx_pathinfo_parse" do
  assert_equal [0, 0, 0, 1, 1],
               AuxTest.pathinfo_parse("a")
  assert_equal [0, 5, 6, 7, 7],
               AuxTest.pathinfo_parse("a/b/c/d")
  assert_equal [0, 5, 6, 7, 9],
               AuxTest.pathinfo_parse("a/b/c/d.e")
  assert_equal [1, 6, 7, 8, 10],
               AuxTest.pathinfo_parse("/a/b/c/d.e")
  assert_equal [1, 6, 7, 12, 12],
               AuxTest.pathinfo_parse("/a/b/c/d.e..")
  assert_equal [1, 6, 7, 8, 8],
               AuxTest.pathinfo_parse("/a/b/c/d")
  assert_equal [1, 1, 0, 1, 1],
               AuxTest.pathinfo_parse("/")
  assert_equal [0, 1, 2, 4, 4],
               AuxTest.pathinfo_parse("a/.b")
  assert_equal [0, 1, 2, 5, 5],
               AuxTest.pathinfo_parse("a/..b")
  assert_equal [0, 1, 2, 4, 6],
               AuxTest.pathinfo_parse("a/b..c")
  assert_equal [0, 1, 2, 4, 6],
               AuxTest.pathinfo_parse("a/b..c/")
  assert_equal [0, 1, 2, 4, 6],
               AuxTest.pathinfo_parse("a/b..c//")
end

if AuxTest.windows?
else
  assert "mrbx_pathinfo_parse (generic)" do
    assert_equal [1, 1, 0, 1, 1],
                 AuxTest.pathinfo_parse("///")
  end
end
