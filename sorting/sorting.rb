#!/usr/bin/ruby2.0


#--- heapsort: in-place implementation with linear-time heap construction

def trickle_down(data, pos, n)
	left = 2*pos+1
	return if left >= n
	right = 2*pos+2
	if right >= n
		if data[pos] < data[left]
			#data[pos], data[left] = data[left], data[pos]
			t = data[pos]
			data[pos] = data[left]
			data[left] = t
		end
		return
	end
	if data[pos] < data[left]
		if data[left] < data[right]
			#data[pos], data[right] = data[right], data[pos]
			t = data[pos]
			data[pos] = data[right]
			data[right] = t
			trickle_down(data, right, n)
		else
			#data[pos], data[left] = data[left], data[pos]
			t = data[pos]
			data[pos] = data[left]
			data[left] = t
			trickle_down(data, left, n)
		end
	elsif data[pos] < data[right]
		#data[pos], data[right] = data[right], data[pos]
		t = data[pos]
		data[pos] = data[right]
		data[right] = t
		trickle_down(data, right, n)
	end
end

def heapsort(data)
	# build a max-heap
	n = data.length
	((n-2)/2).downto(0).each do |i|
		trickle_down(data, i, n)
	end

	# successively remove the max item and swap it to the end
	(n-1).downto(0).each do |i|
		#data[i], data[0] = data[0], data[i]
		t = data[i]
		data[i] = data[0]
		data[0] = t
		trickle_down(data, 0, i)
	end
end

def test_heapsort(data)
	time_block('ran heapsort') { heapsort(data) }
	puts 'error: data is not sorted' if not is_sorted(data)
end


#--- mergesort: recursive implementation with two arrays

def mergesort(data, buffer, left, right)  # half-open interval
	return if left+1 == right
	mid = (left+right)/2
	mergesort(data, buffer, left, mid)
	mergesort(data, buffer, mid, right)
	pos_left = left
	pos_right = mid
	pos_buffer = left
	while true
		if data[pos_left] <= data[pos_right]
			buffer[pos_buffer] = data[pos_left]
            pos_buffer += 1
            pos_left += 1
            if pos_left == mid
                while pos_right != right
                    buffer[pos_buffer] = data[pos_right]
                    pos_buffer += 1
                    pos_right += 1
				end
                break
			end
        else
            buffer[pos_buffer] = data[pos_right]
            pos_buffer += 1
            pos_right += 1
            if pos_right == right
                while pos_left != mid
                    buffer[pos_buffer] = data[pos_left]
                    pos_buffer += 1
                    pos_left += 1
				end
                break
			end
		end
	end
	(left...right).each {|i| data[i] = buffer[i] }
end

def test_mergesort(data)
	time_block('ran mergesort') do
		buffer = Array.new(data.length)
		mergesort(data, buffer, 0, data.length)
	end
	puts 'error: data is not sorted' if not is_sorted(data)
end


#--- quicksort: in-place implementation with median-of-three pivot selection

def quicksort(data, left, right)
    return if left+1 >= right
    #ai, bi, ci = left, (left+right)/2, right-1
	ai = left
    bi = (left+right)/2
	ci = right-1
    #a, b, c = data[ai], data[bi], data[ci]
	a = data[ai]
	b = data[bi]
	c = data[ci]
    if a < b
        if c < a
            pos = ai
        elsif c < b
            pos = ci
        else
            pos = bi
		end
    else
        if c < b
            pos = bi
        elsif c < a
            pos = ci
        else
            pos = ai
		end
	end
    pivot = data[pos]
	data[pos] = data[right-1]
    tail = left
    (left..right-1).each do |i|
        if data[i] < pivot
            #data[tail], data[i] = data[i], data[tail]
			t = data[tail]
			data[tail] = data[i]
			data[i] = t
            tail += 1
		end
	end
    #data[right-1], data[tail] = data[tail], pivot
	data[right-1] = data[tail]
	data[tail] = pivot
    quicksort(data, left, tail)
    quicksort(data, tail+1, right)
end

def test_quicksort(data)
	time_block ('ran quicksort') { quicksort(data, 0, data.length) }
	puts 'error: data is not sorted' if not is_sorted(data)
end


#--- native sort

def test_native_sort(data)
	time_block ('ran native sort') { data.sort! }
	puts 'error: data is not sorted' if not is_sorted(data)
end


#--- student objects

class Student
	include Comparable
	attr_reader :name, :grade
	def initialize(name, grade)
		@name = name
		@grade = grade
	end
	def to_s
		'%d %s' % [@grade, @name]
	end
	def <=>(other)
		if @grade < other.grade
			-1
		elsif @grade > other.grade
			1
		else
			@name <=> other.name
		end
	end
end


#--- random permutation

def shuffle(data)
	# constants for a linear congruential generator
	m = 2**32
	a = 1664525
	c = 1013904223
	random_value = 42  # seed
	(data.length-1).downto(1) do |i|
		random_value = (a*random_value + c) % m
		j = random_value % (i+1)
		if j != i
			t = data[i]
			data[i] = data[j]
			data[j] = t
		end
	end
end


#--- helper functions

def time_block description, &block
	start_time = Time.now.to_f
	block.call
	elapsed = Time.now.to_f - start_time
	puts '    '+description+'  %.3f s' % elapsed
end

def is_sorted(arr)
	(1...arr.length).each {|i| return false if arr[i-1] > arr[i] }
	true
end


def main
	if ARGV.length != 1
		puts 'error: must specify the length of the array'
		exit
	end
	n = ARGV[0].to_i


	#--- integer sorting
	puts 'integers:'

	# initialize an array with 0, ..., n-1
	sorted_integers = nil
	time_block 'made ordered integer array' do
		sorted_integers = (0...n).to_a
	end

	# permute randomly
	shuffled_integers = sorted_integers[0..-1]
	time_block('shuffled') { shuffle(shuffled_integers) }

	# sort integer arrays
	test_heapsort(shuffled_integers[0..-1])
	test_mergesort(shuffled_integers[0..-1])
	test_quicksort(shuffled_integers[0..-1])
	test_native_sort(shuffled_integers[0..-1])


	#--- string sorting
	puts 'strings:'

	# make strings: 'a', 'b', ..., 'z', 'aa', 'ab', ..., 'az', 'ba', ...
	# NB: Ruby can do this with s.next(), but for the sake of comparing
	# languages we'll dispense with this special built-in function.
	sorted_strings = nil
	time_block 'made ordered string array' do
		sorted_strings = Array.new(n)
		next_char = {}
		('a'..'y').each {|ch| next_char[ch] = ch.next }
		chars, num_chars = ['a'], 1
		n.times do |i|
			sorted_strings[i] = chars.join
			pos = num_chars-1
			while true
				if chars[pos] != 'z'
					chars[pos] = next_char[chars[pos]]
					break
				end
				pos -= 1
				if pos == -1
					chars.unshift('a')
					num_chars += 1
					break
				end
			end
			if pos != num_chars-1
				(pos+1..num_chars-1).each {|i| chars[i] = 'a' }
			end
		end
	end

	# permute randomly
	shuffled_strings = sorted_strings[0..-1]
	time_block('shuffled') { shuffle(shuffled_strings) }

	# sort string arrays
	test_quicksort(shuffled_strings[0..-1])
	test_native_sort(shuffled_strings[0..-1])


	#--- object sorting
	puts 'objects:'

	# make student objects based on the sorted strings
	sorted_students = nil
	time_block 'made student objects' do
		sorted_students = sorted_strings.each_with_index.map do |s, i|
			Student.new(s, i%10)
		end
	end

	# permute randomly
	shuffled_students = sorted_students[0..-1]
	time_block('shuffled') { shuffle(shuffled_students) }

	# sort objects
	test_quicksort(shuffled_students[0..-1])
	test_native_sort(shuffled_students[0..-1])

	# extra test with sorting on keys
	data = shuffled_students[0..-1]
	time_block 'sorted natively with keys' do
		data.sort_by! {|student| [student.grade, student.name] }
	end

end

main
