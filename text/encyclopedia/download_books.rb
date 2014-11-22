#!/usr/bin/ruby2.0

require 'open-uri'

index = {}

# The Wikipedia article about E.B. 1911 has a list of Project Gutenberg
# links. Gutenberg itself offers no easy way of finding all parts of the
# encyclopedia. Therefore, we're scraping the Wikipedia list to get the
# Gutenberg book numbers.
text = open('wikipedia.entry.html').read
groups = text.scan(/www.gutenberg\D+(\d+)">Volume\s+([^<]+)\s*</)
groups.each do |parts|
	book_number = parts[0].to_i
	key = parts[1].split('.').map {|s| s.to_i }
	#puts '%s -> %s' % [key, book_number]
	index[key] = book_number
end

# Now we're scraping the Gutenberg index, which doesn't name the encylopedia
# books consistently. However, we can use a pattern to scrape many of the book
# numbers. We're checking these against the book numbers obtained from
# Wikipedia. The hash key is the [volume, slice] pair that we scraped together
# with the book number.
text = open('GUTINDEX.ALL').read
pattern = Regexp.new('Encyclopaedia Britannica, 11th Edition,\s+' +
					 'Volume (\d+), Slice (\d+), by Various\s+(\d+)')
groups = text.scan pattern
groups.each do |group|
	volume, slice, book_number = group.map {|s| s.to_i }
	key = [volume, slice]
	if not index.has_key? key
		puts '%s -> %d found in Gutenberg index, not in Wikipedia list' %
				[key.join('.'), book_number]
		index[key] = book_number
	elsif index[key] != book_number
		puts ('%s -> %d in Gutenberg index ' % [key, book_number]) +
				(' disagrees with Wikipedia\'s %s -> %d' % [key, index[key]])
	end
end

#puts index.entries.sort.map {|key, number|
#	code = key.map {|s| s.to_i }.join '.'
#	'%s %d' % [code, number]
#}.join ', '
#puts index.length

# Now that we have a list of book numbers, we'll download them from a
# Gutenberg mirror. The main Gutenberg site does not allow automated access.
# My IP got banned after a few dozen downloads. Here I'm using the UWaterloo
# mirror, which doesn't seem to mind heavy downloading action.
url_left = 'http://mirror.csclub.uwaterloo.ca/gutenberg/'
index.each do |key, book_number|
	key.push(0) if key.length == 1  # we do this because volume isn't sliced
	code = '%02d.%02d' % key  # our own code: volume 3, slice 1 -> 03.01
	parts = []            # Gutenberg has a directory encoding scheme
	s = book_number.to_s  # e.g., book number 200 is in 2/0/200
	n = s.length          # and book number 30073 is in 3/0/0/7/30073/
	(0..n-2).each {|i| parts.push s[i] }
	parts.push(s)
	url_middle = parts.join '/'
	#puts '%s -> %s' % [code, url_middle]
	
	encoding_choices = {
		'ascii' => { 'suffix' => '.zip', 'directory' => 'zip.ascii' },
		'utf-8' => { 'suffix' => '-8.zip', 'directory' => 'zip.utf-8' }
	}
	encoding = 'ascii'
	suffix = encoding_choices[encoding]['suffix']
	directory = encoding_choices[encoding]['directory']
	url = url_left + url_middle + '/' + s + suffix
	target = directory + '/' + code + suffix
	if File.file? target
		#puts '  %s already exists' % target
		next
	end
	begin
		puts 'downloading '+url
		data = open(url).read
		File.write(target, data)
		puts '  wrote to '+target
	rescue OpenURI::HTTPError => e
		puts '  *** book %s error: %s' % [code, e.to_s]
	end
end
