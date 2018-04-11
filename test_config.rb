MRuby::Build.new do |conf|
  toolchain :clang

  conf.build_dir = "host32"

  cc.defines = %w(MRB_INT32)
  cc.flags << "-Wall"
  cc.flags << "-O0"

  gem core: "mruby-print"
  gem core: "mruby-bin-mirb"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__)

  enable_test
end

MRuby::Build.new("host64") do |conf|
  toolchain :clang

  conf.build_dir = conf.name

  cc.defines = %w(MRB_INT64)
  cc.flags << "-Wall"
  cc.flags << "-O0"

  gem core: "mruby-print"
  gem core: "mruby-bin-mrbc"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__)

  enable_test
end

MRuby::Build.new("host16") do |conf|
  toolchain :clang

  conf.build_dir = conf.name

  cc.defines = %w(MRB_INT16)
  cc.flags << "-Wall"
  cc.flags << "-O0"

  gem core: "mruby-print"
  gem core: "mruby-bin-mrbc"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__)

  enable_test
end

MRuby::Build.new("host++") do |conf|
  toolchain :clang

  conf.build_dir = conf.name

  cc.flags << "-Wall"
  cc.flags << "-pedantic"
  cc.flags << "-std=c++1z"

  gem core: "mruby-print"
  gem core: "mruby-bin-mirb"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__)

  enable_debug
  enable_test
  enable_cxx_abi
end
