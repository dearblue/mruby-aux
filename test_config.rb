MRuby::Build.new("host") do |conf|
  toolchain :clang

  conf.build_dir = "host"

  cc.flags << "-Wall"
  cc.flags << "-O0"

  gem core: "mruby-print"
  gem core: "mruby-bin-mirb"
  gem core: "mruby-bin-mruby"
  gem "."

  enable_test
end

MRuby::Build.new("host64") do |conf|
  toolchain :clang

  conf.build_dir = "host64"

  cc.defines = %w(MRB_INT64)
  cc.flags << "-Wall"
  cc.flags << "-O0"

  gem core: "mruby-print"
  gem core: "mruby-bin-mrbc"
  gem core: "mruby-bin-mruby"
  gem "."

  conf.enable_test
end

MRuby::Build.new("host16") do |conf|
  toolchain :clang

  conf.build_dir = "host16"

  cc.defines = %w(MRB_INT16)
  cc.flags << "-Wall"
  cc.flags << "-O0"

  gem core: "mruby-print"
  gem core: "mruby-bin-mrbc"
  gem core: "mruby-bin-mruby"
  gem "."

  enable_test
end
