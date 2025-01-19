module AuxTest
  class CallSuper1
    def callsuper(a, b, c, &block)
      yield a, b, c
    end
  end

  class CallSuper4 < CallSuper3
    def callsuper(a, b, c, &block)
      super(a, b, c, &block)
    end
  end
end

assert "mrbx_vm_call_super" do
  obj = AuxTest::CallSuper2.new
  assert_equal(6) { obj.callsuper(1, 2, 3) { |x, y, z| x + y + z } }

  obj = AuxTest::CallSuper3.new
  assert_equal(6) { obj.callsuper(1, 2, 3) { |x, y, z| x + y + z } }

  obj = AuxTest::CallSuper4.new
  assert_equal(6) { obj.callsuper(1, 2, 3) { |x, y, z| x + y + z } }
end
