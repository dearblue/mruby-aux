assert "mrbx_need_pathsep_p" do
  assert_equal true, AuxTest.need_pathsep?("a")
  assert_equal true, AuxTest.need_pathsep?("a.b")
  assert_equal true, AuxTest.need_pathsep?("a/b.c")
  assert_equal false, AuxTest.need_pathsep?("a/")
  assert_equal false, AuxTest.need_pathsep?("/")
end

assert "mrbx_pathsep_p" do
  assert_equal true, AuxTest.pathsep?("/")
  assert_equal false, AuxTest.pathsep?("a")
end

assert "mrbx_split_path" do
  assert_equal [0, 0, 0, 1, 1],
               AuxTest.split_path("a")
  assert_equal [0, 5, 6, 7, 7],
               AuxTest.split_path("a/b/c/d")
  assert_equal [0, 5, 6, 7, 9],
               AuxTest.split_path("a/b/c/d.e")
  assert_equal [1, 6, 7, 8, 10],
               AuxTest.split_path("/a/b/c/d.e")
  assert_equal [1, 6, 7, 8, 8],
               AuxTest.split_path("/a/b/c/d")
  assert_equal [1, 1, 0, 1, 1],
               AuxTest.split_path("/")
  assert_equal [0, 1, 2, 4, 4],
               AuxTest.split_path("a/.b")
  assert_equal [0, 1, 2, 5, 5],
               AuxTest.split_path("a/..b")
  assert_equal [0, 1, 2, 4, 6],
               AuxTest.split_path("a/b..c")
  assert_equal [0, 1, 2, 4, 6],
               AuxTest.split_path("a/b..c/")
  assert_equal [0, 1, 2, 4, 6],
               AuxTest.split_path("a/b..c//")
end

if AuxTest.windows?
else
  assert "mrbx_split_path (generic)" do
    assert_equal [1, 1, 0, 1, 1],
                 AuxTest.split_path("///")
  end
end
