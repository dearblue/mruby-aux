#!ruby

assert "cptr" do
  skip if AuxTest::CHECK_CPTR_SKIP
  assert_true AuxTest.check_cptr
end

assert "undef" do
  assert_true AuxTest.check_undef
end

assert "nil" do
  assert_true AuxTest.check_nil
  assert_nil AuxTest.nil
end

assert "false" do
  assert_true AuxTest.check_false
  assert_false AuxTest.false
end

assert "true" do
  assert_true AuxTest.check_true
  assert_true AuxTest.true
end

assert "Fixnum" do
  assert_true AuxTest.check_fixnum
  assert_equal 0, AuxTest.fixnum(0)
  assert_equal 1, AuxTest.fixnum(1)
  assert_equal (-1), AuxTest.fixnum(-1)
  assert_equal (-2), AuxTest.fixnum(-2)
  assert_equal 0x3fffffff, AuxTest.fixnum(0x3fffffff)
end
