#ruby

$: << File.join(MRUBY_ROOT, "lib") # for mruby-1.3 or older

require "mruby/source"

MRuby::Build.new do |conf|
  toolchain :gcc

  conf.build_dir = "host32"

  enable_test

  cc.defines = %w(MRB_INT32)
  cc.flags << "-Wall" << "-O0" << "-std=c11"
  cc.command = "gcc-7"
  cxx.command = "g++-7"

  gem core: "mruby-print"
  gem core: "mruby-bin-mrbc"
  gem File.dirname(__FILE__) do
    include_testtools
  end
end

MRuby::Build.new("host64") do |conf|
  toolchain :gcc

  conf.build_dir = conf.name

  enable_test

  cc.defines = %w(MRB_INT64)
  cc.flags << "-Wall" << "-O0" << "-std=c11"
  cc.command = "gcc-7"
  cxx.command = "g++-7"

  gem core: "mruby-print"
  gem core: "mruby-bin-mrbc"
  gem File.dirname(__FILE__) do
    include_testtools
  end
end

if MRuby::Source::MRUBY_RELEASE_NO > 10200
  MRuby::Build.new("host++") do |conf|
    toolchain :gcc

    conf.build_dir = conf.name

    #enable_debug
    enable_test
    enable_cxx_abi

    cc.flags << "-Wall" << "-pedantic" << "-std=c++1z"
    cc.command = "g++-7"
    cxx.command = "g++-7"

    gem core: "mruby-print"
    gem core: "mruby-bin-mrbc"
    gem File.dirname(__FILE__) do
      include_testtools
    end
  end
end
