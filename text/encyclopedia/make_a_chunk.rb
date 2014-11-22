#!/usr/bin/ruby2.0

if ARGV.length != 1
	puts 'must specify number of tokens desired'
	exit
end
target = eval ARGV[0]
puts 'seeking %d tokens' % target

directory = 'txt.ascii'
out_file_name = 'chunk.txt'

count = 0
pieces = []
Dir.entries(directory).sort.each do |name|
	next if not name.match(/\.ascii$/)
	puts name
	path = directory + '/' + name
	raw = open(path).read.encode('ascii', :invalid=>:replace, :replace=>'?')
	text = raw.match(/\*\*\* START OF THIS.*?\n(.*?)\*\*\* END OF THIS/m)[1]
	pieces.push(text)
	tokens = text.split()
	count += tokens.length
	puts '  + %d = %d' % [tokens.length, count]
	break if count >= target
end

File.write(out_file_name, pieces.join)

puts 'wrote text containing %d tokens to %s' % [count, out_file_name]
