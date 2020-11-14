#ruby

begin
  require "mruby/source"
rescue LoadError
  $: << File.join(MRUBY_ROOT, "lib") # for mruby-1.3 or older
  require "mruby/source"
end

require "yaml"

module Internals
  refine Hash do
    def boxnan?
      [*self["defines"]].include?("MRB_NAN_BOXING")
    end

    def cxxabi?
      !!self["c++abi"]
    end

    def cxxexc?
      !!self["c++exc"]
    end
  end
end

using Internals

config = YAML.load <<'YAML'
  common:
    gems:
    - :core: "mruby-sprintf"
    - :core: "mruby-print"
    - :core: "mruby-bin-mirb"
    - :core: "mruby-bin-mruby"
    - :core: "mruby-bin-mrbc"
  builds:
    host:
      defines: MRB_NO_BOXING
      gems:
      - :core: "mruby-fiber"
    host-nan:
      defines: MRB_NAN_BOXING
    host-word:
      defines: MRB_WORD_BOXING
    host++:
      defines: MRB_NO_BOXING
      c++abi: true
    host++-nan:
      defines: MRB_NAN_BOXING
      c++abi: true
    host++-word:
      defines: MRB_WORD_BOXING
      c++abi: true
    host++exc:
      defines: MRB_NO_BOXING
      c++exc: true
    host++exc-nan:
      defines: MRB_NAN_BOXING
      c++exc: true
    host++exc-word:
      defines: MRB_WORD_BOXING
      c++exc: true
    host32:
      defines: MRB_NO_BOXING
      flags: -m32
    host32-nan:
      defines: MRB_NAN_BOXING
      flags: -m32
    host32-word:
      defines: MRB_WORD_BOXING
      flags: -m32
YAML

config["builds"].each_pair do |n, c|
  next if (c.boxnan? || c.cxxabi? || c.cxxexc?) && MRuby::Source::MRUBY_RELEASE_NO < 10300
  next if (c.boxnan?) && MRuby::Source::MRUBY_RELEASE_NO == 20001

  MRuby::Build.new(n) do |conf|
    toolchain :clang

    conf.build_dir = File.join("build", c["build_dir"] || name)

    enable_debug
    enable_test
    enable_cxx_abi if c["c++abi"]

    cc.defines << [*c["defines"]] << [*c["cdefines"]]
    cxx.defines << [*c["defines"]] << [*c["c++defines"]]
    cc.flags << [*c["flags"]] << [*c["cflags"]]
    cxx.flags << [*c["flags"]] << [*c["c++flags"]]
    linker.flags << [*c["flags"]] << [*c["ldflags"]]

    Array(config.dig("common", "gems")).each { |*g| gem *g }
    Array(c["gems"]).each { |*g| gem *g }

    gem core: "mruby-io" if MRuby::Source::MRUBY_RELEASE_NO >= 10400

    gem __dir__ do |g|
      include_testtools

      if g.cc.command =~ /\\b(?:g?cc|clang)\\d*\\b/
        g.cc.flags << "-std=c11" unless c["c++abi"]
        g.cc.flags << "-pedantic"
        g.cc.flags << %w(-Wall -Wextra)
      end
    end
  end
end
