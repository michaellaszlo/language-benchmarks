#!/usr/bin/ruby2.0


#--- helper functions

def time_block description, &block
	start_time = Time.now.to_f
	block.call
	elapsed = Time.now.to_f - start_time
	puts '    '+description+'  %.3f s' % elapsed
end


def main
	if ARGV.length != 1
		puts 'error: must specify file name'
		exit
	end
	file_name = ARGV[0]

	# read entire file into memory
	text = nil
	time_block 'slurp' do
		text = File.open(file_name).read
	end

	# render to lower case
	time_block 'lowercase' do
		text.downcase!
	end

	# tokenize
	words = nil
	time_block 'tokenize' do
		words = text.split
	end

	# delete non-alphabetic characters from the beginning and end of each token
	junk = Regexp.new(/^\W+|\W+$/)
	time_block 'clean' do
		words.each { |word| word.gsub! junk, '' }
	end

	# count word frequencies
	frequency = Hash.new(0)
	time_block 'count' do
		words.each {|word| frequency[word] += 1 }
	end

	# make a sorted list of tuples for testing purposes
	if true
		counts = frequency.each_pair.sort_by {|word, count| [-count, word] }
		counts[0..4].each {|word, count| puts '%d %s' % [count, word] }
		puts '...'
		counts[-5..-1].each {|word, count| puts '%d %s' % [count, word] }
	end
	
end

main
