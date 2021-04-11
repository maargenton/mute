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
  system "make test" or exit(1)

  success = true
  gold_files = Dir['test/**/gold/*']
  gold_files.each do |gold_file|
    output_file = File.join('build', gold_file).gsub('/gold/', '/')
    puts "Checking '#{output_file}' against gold file ..."
    diff_success = system "diff -B -u #{gold_file} #{output_file}"
    success &&= diff_success
  end
  exit(1) if !success
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

def watch( *glob )
  yield unless block_given?
  files = []
  loop do
    new_files = Dir[*glob].map {|file| File.mtime(file) }
    yield if new_files != files
    files = new_files
    sleep 0.5
  end
end

desc 'Run unit tests everytime a source or test file is changed'
task :watch do
  watch( '**/*.{c,cc,cpp,h,hh,hpp}', 'test/gold/**/*.output' ) do
    success = system "clear && rake"
    print_separator( success )
  end
end
