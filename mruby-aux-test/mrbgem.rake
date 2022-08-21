MRuby::Gem::Specification.new("mruby-aux-test") do |s|
  s.summary = "test for mruby-aux"
  s.license = "CC0"
  s.author  = "dearblue"
  s.homepage = "https://github.com/dearblue/mruby-aux"

  add_dependency "mruby-aux", github: "dearblue/mruby-aux"

  if !build.cxx_abi_enabled? && cc.command =~ /\b(?:g?cc|clang)\d*\b/
    cc.flags << %w(-Wno-declaration-after-statement)
  end

  self.bins = %w(
    mruby-aux-test-mob-create-only
    mruby-aux-test-mob-push
    mruby-aux-test-mob-push-pop
    mruby-aux-test-mob-mix
  )
end
