#ruby

unless Object.const_defined?(:MRUBY_RELEASE_NO)
  if File.read(File.join(MRUBY_BASEDIR, "README.md")) =~ /\bversion\s*\K(\d+)\.(\d+)\.(\d+)\s+/im
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

  cc.defines = %w(MRB_INT32)
  cc.flags << "-Wall" << "-O0" << "-pedantic" << "-std=c11"

  gem core: "mruby-print"
  gem core: "mruby-bin-mirb"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__)
end

MRuby::Build.new("host64") do |conf|
  toolchain :clang

  conf.build_dir = conf.name

  enable_test

  cc.defines = %w(MRB_INT64)
  cc.flags << "-Wall" << "-O0" << "-pedantic" << "-std=c11"

  gem core: "mruby-print"
  gem core: "mruby-bin-mrbc"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__)
end

MRuby::Build.new("host16") do |conf|
  toolchain :clang

  conf.build_dir = conf.name

  enable_test

  cc.defines = %w(MRB_INT16)
  cc.flags << "-Wall" << "-O0" << "-pedantic" << "-std=c11"

  gem core: "mruby-print"
  gem core: "mruby-bin-mrbc"
  gem core: "mruby-bin-mruby"
  gem File.dirname(__FILE__)
end

if MRUBY_RELEASE_NO > 10200
  MRuby::Build.new("host++") do |conf|
    toolchain :clang

    conf.build_dir = conf.name

    enable_test
    enable_cxx_abi

    cc.flags << "-Wall" << "-O0" << "-pedantic" << "-std=c++1z"
    cxx.flags << "-std=c++1z"

    gem core: "mruby-print"
    gem core: "mruby-bin-mirb"
    gem core: "mruby-bin-mruby"
    gem File.dirname(__FILE__)
  end
end
