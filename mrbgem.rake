$LOAD_PATH << File.join(MRUBY_ROOT, "lib")
require "mruby/source"

MRuby::Gem::Specification.new("mruby-aux") do |s|
  s.summary = "auxiliary library for mruby"
  version = File.read(File.join(File.dirname(__FILE__), "README.md")).scan(/^\s*[\-\*] version:\s*(\d+(?:\.\d+)+)/i).flatten[-1]
  s.version = version if version
  s.license = "CC0"
  s.author  = "dearblue"
  s.homepage = "https://github.com/dearblue/mruby-aux"

  add_dependency "mruby-error", core: "mruby-error"

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

  incdir = File.join(__dir__, "include")

  dirp = __dir__.gsub(/[\[\]\{\}\*]/) { |m| "\\#{m}" }
  files = "aux/**/*.c"
  auxobjs = Dir.glob(File.join(dirp, files)).map { |f|
    next nil unless File.file? f
    ff = f.relative_path_from(__dir__)
    objfile File.join(build_dir, ff.pathmap("%X"))
  }.compact

  libmruby_static = MRuby::Source::MRUBY_RELEASE_NO >= 20000 && build.libmruby_static
  libmruby_static ||= build.libfile(File.join(build.build_dir, "lib/libmruby"))

  file libmruby_static => auxobjs
  build.libmruby << auxobjs
end
