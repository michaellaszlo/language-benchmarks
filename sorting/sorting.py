#!/usr/bin/python3

import sys, time, string
from functools import total_ordering


#--- heapsort: in-place implementation with linear-time heap construction

def trickle_down(data, pos, n):
    left = 2*pos+1
    if left >= n:
        return
    right = 2*pos+2
    if right >= n:
        if data[pos] < data[left]:
            data[pos], data[left] = data[left], data[pos]
        return
    if data[pos] < data[left]:
        if data[left] < data[right]:
            data[pos], data[right] = data[right], data[pos]
            trickle_down(data, right, n)
        else:
            data[pos], data[left] = data[left], data[pos]
            trickle_down(data, left, n)
    elif data[pos] < data[right]:
        data[pos], data[right] = data[right], data[pos]
        trickle_down(data, right, n)

def heapsort(data):
    # build a max-heap
    n = len(data)
    for i in range((n-2)//2, -1, -1):
        trickle_down(data, i, n)

    # successively remove the max item and swap it to the end
    for i in range(n-1, -1, -1):
        data[i], data[0] = data[0], data[i]
        trickle_down(data, 0, i)

def test_heapsort(data):
    start_time = time.time()
    heapsort(data)
    elapsed = time.time() - start_time
    print('    ran heapsort  %.3f s' % elapsed)
    if not is_sorted(data):
        print('error: data is not sorted')


#--- mergesort: recursive implementation with two arrays

def mergesort(data, buffer, left, right):  # half-open interval
    if left+1 == right:
        return
    mid = (left+right)//2
    mergesort(data, buffer, left, mid)
    mergesort(data, buffer, mid, right)
    pos_left, pos_right = left, mid
    pos_buffer = left
    while True:
        if data[pos_left] <= data[pos_right]:
            buffer[pos_buffer] = data[pos_left]
            pos_buffer += 1
            pos_left += 1
            if pos_left == mid:
                while pos_right != right:
                    buffer[pos_buffer] = data[pos_right]
                    pos_buffer += 1
                    pos_right += 1
                break
        else:
            buffer[pos_buffer] = data[pos_right]
            pos_buffer += 1
            pos_right += 1
            if pos_right == right:
                while pos_left != mid:
                    buffer[pos_buffer] = data[pos_left]
                    pos_buffer += 1
                    pos_left += 1
                break
    for i in range(left, right):
        data[i] = buffer[i]

def test_mergesort(data):
    start_time = time.time()
    buffer = len(data) * [None]
    mergesort(data, buffer, 0, len(data))
    elapsed = time.time() - start_time
    print('    ran mergesort  %.3f s' % elapsed)
    if not is_sorted(data):
        print('error: data is not sorted')


#--- quicksort: in-place implementation with median-of-three pivot selection

def quicksort(data, left, right):
    if left+1 >= right:
        return
    ai, bi, ci = left, (left+right)//2, right-1
    a, b, c = data[ai], data[bi], data[ci]
    if a < b:
        if c < a:
            pos = ai
        elif c < b:
            pos = ci
        else:
            pos = bi
    else:
        if c < b:
            pos = bi
        elif c < a:
            pos = ci
        else:
            pos = ai
    pivot = data[pos]
    data[pos] = data[right-1]
    tail = left
    for i in range(left, right-1):
        if data[i] < pivot:
            data[tail], data[i] = data[i], data[tail]
            tail += 1
    data[right-1], data[tail] = data[tail], pivot
    quicksort(data, left, tail)
    quicksort(data, tail+1, right)

def test_quicksort(data):
    start_time = time.time()
    quicksort(data, 0, len(data))
    elapsed = time.time() - start_time
    print('    ran quicksort  %.3f s' % elapsed)
    if not is_sorted(data):
        print('error: data is not sorted')


#--- native sort

def test_native_sort(data):
    start_time = time.time()
    data.sort()
    elapsed = time.time() - start_time
    print('    ran native sort  %.3f s' % elapsed)
    if not is_sorted(data):
        print('error: data is not sorted')


#--- student objects

@total_ordering
class Student:
    def __init__(self, name, grade):
        self.name = name
        self.grade = grade

    def __eq__(self, other):
        return (self.grade, self.name) == (other.grade, other.name)

    def __lt__(self, other):
        return (self.grade, self.name) < (other.grade, other.name)

    def __str__(self):
        return '%d %s' % (self.grade, self.name)


#--- random permutation

def shuffle(data):
    # constants for a linear congruential generator
    m = 2**32
    a = 1664525
    c = 1013904223
    random_value = 42  # seed
    for i in range(len(data)-1, 0, -1):
        random_value = (a*random_value + c) % m
        j = random_value % (i+1)
        if j != i:
            data[i], data[j] = data[j], data[i]


#-- helper functions

def is_sorted(arr):
    for i in range(1, len(arr)):
        if arr[i-1] > arr[i]:
            return False
    return True

def display(data):
    print(' '.join([str(x) for x in data]))


def main():
    if len(sys.argv) != 2:
        print('error: must specify the length of the array')
        sys.exit()
    n = int(sys.argv[1])


    #--- integer sorting
    print('integers:')

    # initialize an array with 0, ..., n-1
    start_time = time.time()
    sorted_integers = list(range(n))
    elapsed = time.time() - start_time
    print('    made ordered integer array  %.3f s' % elapsed)

    # permute it randomly
    shuffled_integers = sorted_integers[:]
    start_time = time.time()
    shuffle(shuffled_integers)
    elapsed = time.time() - start_time
    print('    shuffled  %.3f s' % elapsed)

    # sort integer arrays
    test_heapsort(shuffled_integers[:])
    test_mergesort(shuffled_integers[:])
    test_quicksort(shuffled_integers[:])
    test_native_sort(shuffled_integers[:])


    #--- string sorting
    print('strings:')

    # make strings: 'a', 'b', ..., 'z', 'aa', 'ab', ..., 'az', 'ba', ...
    start_time = time.time()
    sorted_strings = n * [None]
    next_char = {}
    alpha = string.ascii_lowercase
    for i in range(25):
        next_char[alpha[i]] = alpha[i+1]
    chars, num_chars = ['a'], 1
    for i in range(n):
        sorted_strings[i] = ''.join(chars)
        pos = num_chars-1
        while True:
            if chars[pos] != 'z':
                chars[pos] = next_char[chars[pos]]
                break
            pos -= 1
            if pos == -1:
                chars.insert(0, 'a')
                num_chars += 1
                break
        if pos != num_chars-1:
            for i in range(pos+1, num_chars):
                chars[i] = 'a'
    elapsed = time.time() - start_time
    print('    made ordered string array  %.3f s' % elapsed)

    # permute it randomly
    shuffled_strings = sorted_strings[:]
    start_time = time.time()
    shuffle(shuffled_strings)
    elapsed = time.time() - start_time
    print('    shuffled  %.3f s' % elapsed)

    # sort string arrays
    test_quicksort(shuffled_strings[:])
    test_native_sort(shuffled_strings[:])

    
    #--- object sorting
    print('objects:')

    # make student objects based on the sorted strings
    start_time = time.time()
    sorted_students = [Student(s, i%10) for i, s in enumerate(sorted_strings)]
    elapsed = time.time() - start_time
    print('    made student objects  %.3f s' % elapsed)

    # permute randomly
    shuffled_students = sorted_students[:]
    start_time = time.time()
    shuffle(shuffled_students)
    elapsed = time.time() - start_time
    print('    shuffled  %.3f s' % elapsed)

    # sort objects
    test_quicksort(shuffled_students[:])
    test_native_sort(shuffled_students[:])

    # extra test with sorting on keys
    data = shuffled_students[:]
    start_time = time.time()
    data.sort(key=lambda student: (student.grade, student.name))
    elapsed = time.time() - start_time
    print('    sorted natively with keys  %.3f s' % elapsed)

main()
