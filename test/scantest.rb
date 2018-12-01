#!ruby

assert "no keyword" do
  AuxScanHashTest.scan_static_keywords(nil, nil, [nil, 1, 2.0, true]) do |rest, a, b, c, d|
    assert_equal nil, rest
    assert_equal nil, a
    assert_equal 1, b
    assert_equal 2.0, c
    assert_equal true, d
  end
end

assert "no keyword with rest" do
  rest0 = {}

  AuxScanHashTest.scan_static_keywords(nil, rest0, [nil, 1, 2.0, true]) do |rest, a, b, c, d|
    assert_equal rest0.object_id, rest.object_id
    assert_equal ({}), rest
    assert_equal nil, a
    assert_equal 1, b
    assert_equal 2.0, c
    assert_equal true, d
  end
end

assert "no keyword with rest as true" do
  AuxScanHashTest.scan_static_keywords(nil, true, [nil, 1, 2.0, true]) do |rest, a, b, c, d|
    assert_equal ({}), rest
    assert_equal nil, a
    assert_equal 1, b
    assert_equal 2.0, c
    assert_equal true, d
  end
end

assert "no keyword with wrong rest type" do
  rest0 = 1

  assert_raise(TypeError) {
    AuxScanHashTest.scan_static_keywords(nil, rest0, [nil, 1, 2.0, true])
  }
end

assert "scan keyword" do
  target = { a: 100, b: "abcdefg" }

  AuxScanHashTest.scan_static_keywords(target, nil, [nil, 1, 2.0, true]) do |rest, a, b, c, d|
    assert_equal nil, rest
    assert_equal 100, a
    assert_equal "abcdefg", b
    assert_equal 2.0, c
    assert_equal true, d
  end
end

assert "scan keyword (raise on rest)" do
  target = { unnecessary: -1 }

  assert_raise(ArgumentError) {
    AuxScanHashTest.scan_static_keywords(target, nil, [nil, 1, 2.0, true])
  }
end

assert "scan keyword (catch rest)" do
  target = { unnecessary1: -1, unnecessary2: "ABCDEFG", a: 100, b: "abcdefg" }

  AuxScanHashTest.scan_static_keywords(target, true, [nil, 1, 2.0, true]) do |rest, a, b, c, d|
    assert_equal ({ unnecessary1: -1, unnecessary2: "ABCDEFG" }), rest
    assert_equal 100, a
    assert_equal "abcdefg", b
    assert_equal 2.0, c
    assert_equal true, d
  end
end

assert "wrong target" do
  assert_raise(TypeError) {
    AuxScanHashTest.scan_static_keywords(1, nil, [nil, 1, 2.0, true])
  }
end

assert "requirement keyword" do
  target = { a: 100 }

  AuxScanHashTest.scan_static_keywords(target, nil, [AuxScanHashTest::REQUIREMENT_KEYWORD, 1, 2.0, true]) do |rest, a, b, c, d|
    assert_equal 100, a
    assert_equal 1, b
    assert_equal 2.0, c
    assert_equal true, d
  end
end

assert "missing keyword from hash" do
  target = { b: 5, c: 10, d: 15 }

  assert_raise(ArgumentError) {
    AuxScanHashTest.scan_static_keywords(target, nil, [AuxScanHashTest::REQUIREMENT_KEYWORD, 1, 2.0, true])
  }
end

assert "missing keyword from nil" do
  assert_raise(ArgumentError) {
    AuxScanHashTest.scan_static_keywords(nil, nil, [AuxScanHashTest::REQUIREMENT_KEYWORD, 1, 2.0, true])
  }
end
