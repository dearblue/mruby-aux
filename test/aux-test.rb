#!ruby

class AuxTest::StringIO_no_youna_mono
  attr_accessor :string
  attr_accessor :pos

  def initialize(string)
    @string = string
    @pos = 0
  end

  def read(size, buf = "")
    d = @string.slice(pos, size)

    return nil if d.nil? || d.empty?

    raise "wrong slice size" if d.size > size

    if buf
      buf.replace d
    else
      buf = d
    end

    @pos += d.size

    buf
  end
end

assert "mrbx_str_drop" do
  str = "abcdefg"
  assert_equal "bcdefg", AuxTest.str_drop(str.dup, 0, 1)
  assert_equal "cdefg", AuxTest.str_drop(str.dup, 0, 2)
  assert_equal "", AuxTest.str_drop(str.dup, 0, 100)
  assert_equal "abcdefg", AuxTest.str_drop(str.dup, 0, -1)
  assert_equal "abdefg", AuxTest.str_drop(str.dup, 2, 1)
  assert_equal "abefg", AuxTest.str_drop(str.dup, 2, 2)
  assert_equal "ab", AuxTest.str_drop(str.dup, 2, 100)
  assert_equal "abcdefg", AuxTest.str_drop(str.dup, 2, -2)
  assert_equal "abcdef", AuxTest.str_drop(str.dup, 6, 1)
  assert_equal "abcdef", AuxTest.str_drop(str.dup, 6, 2)
  assert_equal "abcdef", AuxTest.str_drop(str.dup, 6, 100)
  assert_equal "abcdefg", AuxTest.str_drop(str.dup, 6, -2)
  assert_equal "abcdefg", AuxTest.str_drop(str.dup, 7, 1)
  assert_equal "abcdefg", AuxTest.str_drop(str.dup, 7, 2)
  assert_equal "abcdefg", AuxTest.str_drop(str.dup, 7, 100)
  assert_equal "abcdefg", AuxTest.str_drop(str.dup, 7, -2)
  assert_equal "abcdef", AuxTest.str_drop(str.dup, -1, 1)
  assert_equal "abcdef", AuxTest.str_drop(str.dup, -1, 2)
  assert_equal "abcdef", AuxTest.str_drop(str.dup, -1, 100)
  assert_equal "abcdefg", AuxTest.str_drop(str.dup, -1, -2)
  assert_equal "bcdefg", AuxTest.str_drop(str.dup, -7, 1)
  assert_equal "cdefg", AuxTest.str_drop(str.dup, -7, 2)
  assert_equal "", AuxTest.str_drop(str.dup, -7, 100)
  assert_equal "abcdefg", AuxTest.str_drop(str.dup, -7, -2)
  assert_raise(IndexError) { AuxTest.str_drop(str.dup, -8, 1) }
end

assert "mrbx_fakedin (string)" do
  strsrc = "abcdefghijklmnopqrstuvwxyz" * 99
  fin = AuxTest::FakedIn.new(strsrc.freeze)
  assert_equal strsrc.hash, fin.read(strsrc.bytesize).hash

  fin = AuxTest::FakedIn.new(strsrc.freeze)
  assert_equal strsrc.hash, fin.read(strsrc.bytesize + 1).hash

  fin = AuxTest::FakedIn.new(strsrc.freeze)
  assert_equal strsrc.slice(0 .. -2).hash, fin.read(strsrc.bytesize - 1).hash
  assert_equal strsrc.slice(-1, 1), fin.read(99)

  fin = AuxTest::FakedIn.new(strsrc.freeze)
  assert_equal strsrc.slice(0, 50), fin.read(50)
  assert_equal strsrc.slice(50, 100), fin.read(100)
  assert_equal strsrc.slice(150, 200).hash, fin.read(200).hash
  assert_equal strsrc.slice(350 .. -1).hash, fin.read(10000).hash
  assert_equal nil.hash, fin.read(10000).hash

  fin = AuxTest::FakedIn.new("")
  assert_equal false, fin.eof?
  assert_equal 0, fin.tell
  assert_equal nil, fin.read(1000)
  assert_equal true, fin.eof?
  assert_equal 0, fin.tell
end

assert "mrbx_fakedin (stream)" do
  strsrc = "abcdefghijklmnopqrstuvwxyz" * 99

  stream = AuxTest::StringIO_no_youna_mono.new(strsrc.freeze)
  fin = AuxTest::FakedIn.new(stream)
  assert_equal strsrc.hash, fin.read(strsrc.bytesize).hash

  stream = AuxTest::StringIO_no_youna_mono.new(strsrc.freeze)
  fin = AuxTest::FakedIn.new(stream)
  assert_equal strsrc.hash, fin.read(strsrc.bytesize + 1).hash

  stream = AuxTest::StringIO_no_youna_mono.new(strsrc.freeze)
  fin = AuxTest::FakedIn.new(stream)
  assert_equal strsrc.slice(0 .. -2).hash, fin.read(strsrc.bytesize - 1).hash
  assert_equal strsrc.slice(-1, 1), fin.read(99)

  stream = AuxTest::StringIO_no_youna_mono.new(strsrc.freeze)
  fin = AuxTest::FakedIn.new(stream)
  assert_equal strsrc.slice(0, 50), fin.read(50)
  assert_equal strsrc.slice(50, 100), fin.read(100)
  assert_equal strsrc.slice(150, 200).hash, fin.read(200).hash
  assert_equal strsrc.slice(350 .. -1).hash, fin.read(10000).hash
  assert_equal nil.hash, fin.read(10000).hash
end
