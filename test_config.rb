MRuby::Build.new do |conf|
  toolchain :clang

  conf.build_dir = "host32"

  enable_debug
  enable_test

  cc.flags << "-Wall" << "-pedantic" << "-std=c11"

  gem core: "mruby-print"
  gem core: "mruby-bin-mirb"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__)
end

MRuby::Build.new("host32++") do |conf|
  toolchain :clang

  conf.build_dir = "host32++"

  enable_debug
  enable_test
  enable_cxx_abi

  cc.flags << "-Wall" << "-pedantic" << "-std=c++1z"

  gem core: "mruby-print"
  gem core: "mruby-bin-mirb"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__)
end
