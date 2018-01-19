MRuby::Gem::Specification.new("mruby-aux") do |s|
  s.summary = "auxiliary library for mruby"
  s.version = "0.1"
  s.license = "BSD-2-Clause"
  s.author  = "dearblue"
  s.homepage = "https://github.com/dearblue/mruby-aux"

  if cc.command =~ /\b(?:g?cc|clang)\d*\b/
    cc.flags << "-Wno-shift-negative-value"
    cc.flags << "-Wno-shift-count-negative"
    cc.flags << "-Wno-shift-count-overflow"
    cc.flags << "-Wno-missing-braces"
  end
end
