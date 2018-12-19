#ruby

unless Object.const_defined?(:MRUBY_RELEASE_NO)
  if File.read(File.join(MRUBY_ROOT, "README.md")) =~ /\bversion\s*\K(\d+)\.(\d+)\.(\d+)\s+/im
    MRUBY_RELEASE_NO = $1.to_i * 10000 + $2.to_i * 100 + $3.to_i
  else
    warn "mruby version not found! temporary version number is set to 1.0.0"
    MRUBY_RELEASE_NO = 10000
  end
end

MRuby::Build.new do |conf|
  toolchain :clang

  conf.build_dir = "host32"

  enable_test
  enable_debug

  cc.defines = %w(MRB_INT32)

  gem core: "mruby-print"
  gem core: "mruby-bin-mirb"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__) do
    cc.flags << %w(-std=c11 -Wall -pedantic)
  end
end

MRuby::Build.new("host64") do |conf|
  toolchain :clang

  conf.build_dir = conf.name

  enable_test
  enable_debug

  cc.defines = %w(MRB_INT64)

  gem core: "mruby-print"
  gem core: "mruby-bin-mrbc"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__) do
    cc.flags << %w(-std=c11 -Wall -pedantic)
  end
end

MRuby::Build.new("host16") do |conf|
  toolchain :clang

  conf.build_dir = conf.name

  enable_test
  enable_debug

  cc.defines = %w(MRB_INT16)

  gem core: "mruby-print"
  gem core: "mruby-bin-mrbc"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__) do
    cc.flags << %w(-std=c11 -Wall -pedantic)
  end
end

if MRUBY_RELEASE_NO >= 10300
  MRuby::Build.new("host++") do |conf|
    toolchain :clang

    conf.build_dir = conf.name

    enable_test
    enable_debug
    enable_cxx_abi

    gem core: "mruby-print"
    gem core: "mruby-bin-mirb"
    gem core: "mruby-bin-mruby"
    gem File.dirname(__FILE__) do
      stdcxx = (MRUBY_RELEASE_NO < 10400 ? "c++11" : "c++1z")

      cc.flags << %W(-Wall -pedantic -std=#{stdcxx})
      cxx.flags << "-std=#{stdcxx}"
    end
  end
end
