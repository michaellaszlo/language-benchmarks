#!/usr/bin/ruby2.0

encoding = 'ascii'
source_dir= 'zip.' + encoding
unpack_dir = 'unpack'
target_dir = 'txt.' + encoding

Dir.entries(source_dir).sort.each do |name|
	next if not name.match(/zip$/)

	unzip = 'unzip %s/%s -d %s' % [source_dir, name, unpack_dir]
	IO.popen(unzip).read  # discard the messages from unzip

	temp_name = nil
	Dir.entries(unpack_dir).each do |name|
		if name.match(/^.*\.txt$/)
			temp_name = name
			break
		end
	end
	if temp_name == nil
		puts 'error determining file name'
		exit
	end
	new_name = name.sub(/zip$/, encoding)
	puts '%s -> %s' % [temp_name, new_name]
	old_path = unpack_dir+'/'+temp_name
	new_path = target_dir+'/'+new_name
	File.rename(old_path, new_path)

	command = 'dos2unix '+new_path
	system command
end
