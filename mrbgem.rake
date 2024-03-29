MRuby::Gem::Specification.new("mruby-aux") do |s|
  s.summary = "auxiliary library for mruby"
  version = File.read(File.join(__dir__, "README.md")).scan(/^\s*[\-\*] version:\s*(\d+(?:\.\d+)+)/i).flatten[-1]
  s.version = version if version
  s.license = "CC0"
  s.author  = "dearblue"
  s.homepage = "https://github.com/dearblue/mruby-aux"

  add_dependency "mruby-error", core: "mruby-error"

  if !build.cxx_abi_enabled? && cc.command =~ /\b(?:g?cc|clang)\d*\b/
    cc.flags << %w(-Wno-declaration-after-statement)
  end
end
