#!/usr/bin/python3

import sys, time, re, collections

def main():
    if len(sys.argv) != 2:
        print('error: must specify file name')
        sys.exit()
    file_name = sys.argv[1]

    # read entire file into memory
    start_time = time.time()
    text = open(file_name).read()
    elapsed = time.time() - start_time
    print('    slurp  %.3f s' % elapsed)

    # render to lower case
    text = text.lower()
    start_time = time.time()
    elapsed = time.time() - start_time
    print('    lowercase  %.3f s' % elapsed)

    # tokenize
    start_time = time.time()
    words = text.split()
    elapsed = time.time() - start_time
    print('    tokenize  %.3f s' % elapsed)

    # delete non-alphabetic characters from the beginning and end of each token
    start_time = time.time()
    junk = re.compile('^\W|\W$')
    words = [ junk.sub('', word) for word in words ]
    elapsed = time.time() - start_time
    print('    clean  %.3f s' % elapsed)

    # count word frequencies
    start_time = time.time()
    frequency = collections.defaultdict(lambda: 0)
    for word in words:
        frequency[word] += 1
    elapsed = time.time() - start_time
    print('    count  %.3f s' % elapsed)

    # make a sorted list of tuples for testing purposes
    if True:
        counts = sorted(list(frequency.items()),
                key=lambda pair: (-pair[1], pair[0]))
        for word, count in counts[:5]:
            print('%d %s' % (count, word))
        print('...')
        for word, count in counts[-5:]:
            print('%d %s' % (count, word))
    
main()
