MRuby::Gem::Specification.new("mruby-aux") do |s|
  s.summary = "auxiliary library for mruby"
  s.version = File.read(File.join(File.dirname(__FILE__), "README.md")).scan(/^\s*[\-\*] version:\s*(\d+(?:\.\d+)+)/i).flatten[-1]
  s.license = "CC0"
  s.author  = "dearblue"
  s.homepage = "https://github.com/dearblue/mruby-aux"

  add_dependency "mruby-error", core: "mruby-error"

  has_mrb_hash_foreach = File.read(File.join(MRUBY_ROOT, "include/mruby/hash.h")) =~ /\bmrb_hash_foreach\b/

  cc.defines << "MRUBY_AUX_NEED_HASH_FOREACH" unless has_mrb_hash_foreach

  class << self
    def include_testtools
      self.bins = %w(
        mruby-aux-test-mob-create-only
        mruby-aux-test-mob-push
        mruby-aux-test-mob-push-pop
        mruby-aux-test-mob-mix
      )
    end
  end
end
