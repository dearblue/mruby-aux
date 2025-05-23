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

    def boxword?
      [*self["defines"]].include?("MRB_WORD_BOXING")
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
  builds:
    host:
      defines: MRB_NO_BOXING
      gems:
      - :core: mruby-bin-mrbc
    host-c99:
      defines: MRB_NO_BOXING
      c99: true
      gems:
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
YAML

MRuby::Lockfile.disable rescue nil

config["builds"].each_pair do |n, c|
  next if MRuby::Source::MRUBY_RELEASE_NO < 20100 && (c.boxnan? || c.boxword? || c.cxxabi? || c.cxxexc?)

  MRuby::Build.new(n) do |conf|
    toolchain (ENV["CC"] =~ /clang/ ? "clang" : "gcc")

    conf.build_dir = File.join("build", c["build_dir"] || name)

    enable_debug
    enable_test
    enable_bintest
    enable_cxx_abi if c["c++abi"]

    cc.defines << [*c["defines"]] << [*c["cdefines"]]
    cxx.defines << [*c["defines"]] << [*c["c++defines"]]
    cc.flags << [*c["flags"]] << [*c["cflags"]]
    cxx.flags << [*c["flags"]] << [*c["c++flags"]]
    linker.flags << [*c["flags"]] << [*c["ldflags"]]

    if MRuby::Source::MRUBY_RELEASE_NO < 30000 && !cxx_abi_enabled? && cc.command =~ /\b(?:g?cc|clang)\d*\b/
      cc.flags << %w(-Wno-declaration-after-statement)
    end

    Array(config.dig("common", "gems")).each { |*g| gem *g }
    Array(c["gems"]).each { |*g| gem *g }

    if MRuby::Source::MRUBY_RELEASE_NO >= 10400
      gem core: "mruby-io"
    else
      gem core: "mruby-print"
    end

    [__dir__].each do |gdir|
      gem gdir do |g|
        if g.cc.command =~ /\b(?:g?cc|clang)\d*\b/
          g.cxx.flags << "-std=c++11"
          g.cxx.flags << %w(-Wpedantic -Wall -Wextra)
          case
          when c["c++abi"]
            g.cc.flags << "-std=c++11"
          when c["c99"]
            g.cc.flags << "-std=c99"
          else
            g.cc.flags << "-std=c11"
          end
          g.cc.flags << %w(-Wpedantic -Wall -Wextra -Wno-unused-parameter)
          unless ENV["CC"] =~ /gcc/
            g.cc.flags << "-Wno-newline-eof"
          end
        end
      end
    end
  end
end
