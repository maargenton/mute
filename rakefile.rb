# =============================================================================
#
# MODULE      : rakefile.rb
# PROJECT     : FolderTemplate
# DESCRIPTION :
#
# Copyright (c) 2016, Marc-Antoine Argenton.  All rights reserved.
# =============================================================================

require 'pp'
require 'fileutils'
require 'open3'

task default: [:test]

desc 'Build the primary binary, test binaries and gather test results'
task :make do
  system "make"
end

desc 'Build and run the primary binary'
task :run do
  system "make run"
end

desc 'Build test binaries, gather test results and compare them to golden files'
task :test do
  system "make test"
  gold_files = Dir['test/**/gold/*']
  gold_files.each do |gold_file|
    output_file = File.join('build', gold_file).gsub('/gold/', '/')
    puts "Checking '#{output_file}' against gold file ..."
    system "diff -B -u #{gold_file} #{output_file}" or exit!(1)
  end
end

desc 'Build and run test binaries, updates golden files from output'
task :'update-gold' do
  system "make test"
  files = Dir['build/test/**/*.test.output']
  output_dir = 'test/gold'
  FileUtils.makedirs( output_dir )
  files.each do |f|
    cp f, output_dir
  end

end

desc 'Remove all build products (build folder)'
task :clean do
  system "make clean"
end



# ----------------------------------------------------------------------------
# Definitions to help formating 'rake watch' results
# ----------------------------------------------------------------------------

TERM_WIDTH = `tput cols`.to_i || 80

def tty_red(str);           "\e[31m#{str}\e[0m" end
def tty_green(str);         "\e[32m#{str}\e[0m" end
def tty_blink(str);         "\e[5m#{str}\e[25m" end
def tty_reverse_color(str); "\e[7m#{str}\e[27m" end

def print_separator( success = true )
  if success
    puts tty_green( "-" * TERM_WIDTH )
  else
    puts tty_reverse_color(tty_red( "-" * TERM_WIDTH ))
  end
end



# ----------------------------------------------------------------------------
# Definition of watch task, that monitors the project folder for any relevant
# file change and runs the unit test of the project.
# ----------------------------------------------------------------------------

begin
  require 'watch'

  desc 'Run unit tests everytime a source or test file is changed'
  task :watch do
    Watch.new( '**/*.{c,cc,cpp,h,hh,hpp,gold}' ) do
      success = system "clear && rake"
      print_separator( success )
    end
  end

rescue LoadError

  desc 'Run unit tests everytime a source or test file is changed'
  task :watch do
    puts
    puts "'rake watch' requires the watch gem to be available"
    puts
    puts "To install:"
    puts "    gem install watch"
    puts " or "
    puts "    sudo gem install watch"
    puts
    fail
  end
end
