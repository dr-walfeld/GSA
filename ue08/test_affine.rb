#!/usr/bin/env ruby

if ARGV.length != 1 then
  STDERR.puts "Usage: #{$0} <program>"
  exit 1
end

seq = {}
1.upto(4) do |i|
  #seq[i] = File.open("seq#{i}.fas").readlines.last.chomp
  #call program with file names not contents
  seq[i] = "seq#{i}.fas"
end

solution = File.open("affinealign.out").readlines

k = 0
1.upto(4) do |i|
  1.upto(4) do |j|
    out = %x[#{ARGV[0]} #{seq[i]} #{seq[j]} 3 1]
    if out != solution[k*3..k*3+2].join
      puts out, solution[k*3..k*3+2]
      raise "alignments of seq#{i} and seq#{j} do not match!"
    end
    k += 1
  end
end
