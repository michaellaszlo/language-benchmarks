

//--- heapsort: in-place implementation with linear-time heap construction

function trickleDown(data, pos, n) {
	var left = 2*pos+1;
	if (left >= n) {
		return;
	}
	var right = 2*pos+2;
	if (right >= n) {
		if (data[pos] < data[left]) {
			var t = data[pos];
			data[pos] = data[left];
			data[left] = t;
		}
		return;
	}
	if (data[pos] < data[left]) {
		if (data[left] < data[right]) {
			var t = data[pos];
			data[pos] = data[right];
			data[right] = t;
			trickleDown(data, right, n);
		} else {
			var t = data[pos];
			data[pos] = data[left];
			data[left] = t;
			trickleDown(data, left, n);
		}
	} else if(data[pos] < data[right]) {
		var t = data[pos];
		data[pos] = data[right];
		data[right] = t;
		trickleDown(data, right, n);
	}
}

function heapsort(data) {
	// build a max-heap
	var n = data.length;
	for (var i = Math.floor((n-2)/2); i != -1; --i) {
		trickleDown(data, i, n);
	}

	// successively remove the max item and swap it to the end
	for (var i = n-1; i != -1; --i) {
		var t = data[i];
		data[i] = data[0];
		data[0] = t;
		trickleDown(data, 0, i);
	}
}

function testHeapsort(data) {
	timeFunction('ran heapsort', function () {
		heapsort(data);
	});
	if (!isSorted(data)) {
		print('error: data is not sorted');
	}
}


//--- mergesort: recursive implementation with two arrays

function mergesort(data, buffer, left, right) {
	if (left+1 == right) {
		return;
	}
	var mid = Math.floor((left+right)/2);
	mergesort(data, buffer, left, mid);
	mergesort(data, buffer, mid, right);
	var leftPos = left, rightPos = mid,
		bufferPos = left;
	while (true) {
		if (data[leftPos] <= data[rightPos]) {
			buffer[bufferPos++] = data[leftPos++];
			if (leftPos == mid) {
				while (rightPos != right) {
					buffer[bufferPos++] = data[rightPos++];
				}
				break;
			}
		} else {
			buffer[bufferPos++] = data[rightPos++];
			if (rightPos == right) {
				while (leftPos != mid) {
					buffer[bufferPos++] = data[leftPos++];
				}
				break;
			}
		}
	}
	for (var i = left; i != right; ++i) {
		data[i] = buffer[i];
	}
}

function testMergesort(data) {
	timeFunction('ran mergesort', function () {
		var buffer = new Array(data.length);
		mergesort(data, buffer, 0, data.length);
	});
	if (!isSorted(data)) {
		print('error: data is not sorted');
	}
}


//--- quicksort: in-place implementation with median-of-three pivot selection

function quicksort(data, left, right) {
	if (left+1 >= right) {
		return;
	}
	var ai = left, bi = Math.floor((left+right)/2), ci = right-1,
		a = data[ai], b = data[bi], c = data[ci],
		pos;
    if (a < b) {
        if (c < a) {
            pos = ai;
		} else if (c < b) {
			pos = ci;
		} else {
			pos = bi;
		}
	} else {
        if (c < b) {
            pos = bi;
		} else if (c < a) {
			pos = ci;
		} else {
			pos = ai;
		}
	}
    var pivot = data[pos];
	data[pos] = data[right-1];
	var tail = left;
	for (var i = left; i != right-1; ++i) {
		if (data[i] < pivot) {
			var t = data[tail];
			data[tail] = data[i];
			data[i] = t;
			++tail;
		}
	}
	data[right-1] = data[tail];
	data[tail] = pivot;
    quicksort(data, left, tail);
    quicksort(data, tail+1, right);
}

function testQuicksort(data) {
	timeFunction('ran quicksort', function () {
		quicksort(data, 0, data.length);
	});
	if (!isSorted(data)) {
		print('error: data is not sorted');
	}
}

function quicksortCustom(data, left, right, compare) {
	if (left+1 >= right) {
		return;
	}
	var ai = left, bi = Math.floor((left+right)/2), ci = right-1,
		a = data[ai], b = data[bi], c = data[ci],
		pos;
    if (compare(a, b) < 0) {
        if (compare(c, a) < 0) {
            pos = ai;
		} else if (compare(c, b) < 0) {
			pos = ci;
		} else {
			pos = bi;
		}
	} else {
		if (compare(c, b) < 0) {
            pos = bi;
		} else if (compare(c, a) < 0) {
			pos = ci;
		} else {
			pos = ai;
		}
	}
    var pivot = data[pos];
	data[pos] = data[right-1];
	var tail = left;
	for (var i = left; i != right-1; ++i) {
		if (compare(data[i], pivot) < 0) {
			var t = data[tail];
			data[tail] = data[i];
			data[i] = t;
			++tail;
		}
	}
	data[right-1] = data[tail];
	data[tail] = pivot;
    quicksortCustom(data, left, tail, compare);
    quicksortCustom(data, tail+1, right, compare);
}

function testQuicksortCustom(data, compare) {
	timeFunction('ran quicksort', function () {
		quicksortCustom(data, 0, data.length, compare);
	});
	if (!isSortedCustom(data, compare)) {
		print('error: data is not sorted');
	}
}


//--- native sort

function testNativeSort(data, compare) {
	if (compare) {
		var fun = function () {
			data.sort(compare);
		}
	} else {
		var fun = function () {
			data.sort();
		}
	}
	timeFunction('ran native sort', fun);
	if (!isSorted(data)) {
		print('error: data is not sorted');
	}
}


//--- random permutation

function shuffle(data) {
	// constants for a linear congruential generator
	var m = Math.pow(2, 32),
		a = 1664525,
		c = 1013904223,
		randomValue = 42;  // seed
	for (var i = data.length-1; i != 0; --i) {
		randomValue = (a*randomValue + c) % m;
		var j = randomValue % (i+1);
		if (j != i) {
			var t = data[i];
			data[i] = data[j];
			data[j] = t;
		}
	}
}


//--- helper functions

function timeFunction(description, fun) {
	var startTime = performance.now();
	fun();
	var elapsed = (performance.now() - startTime)/1000;
	print('    '+description+'  '+formatF(elapsed, 3)+' s');
}

function isSorted(data) {
	var n = data.length;
	for (var i = 1; i != n; ++i) {
		if (data[i-1] > data[i]) {
			return false;
		}
	}
	return true;
}

function isSortedCustom(data, compare) {
	var n = data.length;
	for (var i = 1; i != n; ++i) {
		if (compare(data[i-1], data[i]) > 0) {
			return false;
		}
	}
	return true;
}

function displayStudents(students) {
	var parts = [], n = students.length;
	for (var i = 0; i < n; ++i) {
		parts.push(students[i].grade+' '+students[i].name);
	}
	print(parts.join(' '));
}

function formatF(value, digits) {
	var whole = Math.floor(value),
		decimal = ''+Math.round(Math.pow(10, digits)*(value-whole));
	decimal = decimal+'00000000000000000000'.slice(0, digits-decimal.length);
	return whole+"."+decimal;
}


function main() {
	var n = parseInt(readline());
	if (!(n >= 1)) {
		print('error: must specify the length of the array');
		return;
	}


	//--- integer sorting
	print('integers:');

	// initialize an array with 0, ..., n-1
	var sortedIntegers;
	timeFunction('made ordered integer array', function () {
		sortedIntegers = new Array(n);
		for (var i = 0; i < n; ++i) {
			sortedIntegers[i] = i;
		}
	});

	// permute randomly
	var shuffledIntegers = sortedIntegers.slice();
	timeFunction('shuffled', function () {
		shuffle(shuffledIntegers);
	});

	// sort integer arrays
	testHeapsort(shuffledIntegers.slice());
	testMergesort(shuffledIntegers.slice());
	testQuicksort(shuffledIntegers.slice());
	testNativeSort(shuffledIntegers.slice(), function (a, b) {
		return a-b;
	});


	//--- string sorting
	print('strings:');

	// make strings: 'a', 'b', ..., 'z', 'aa', 'ab', ..., 'az', 'ba', ...
	var sortedStrings;
	timeFunction('made ordered string array', function () {
		sortedStrings = new Array(n);
		var next_char = {}, zCode = 'z'.charCodeAt(0);
		for (var code = 'a'.charCodeAt(0); code != zCode; ++code) {
			next_char[String.fromCharCode(code)] = String.fromCharCode(code+1);
		}
		var chars = ['a'], numChars = 1;
		for (var i = 0; i != n; ++i) {
			sortedStrings[i] = chars.join('');
			var pos = numChars-1;
			while (true) {
				if (chars[pos] != 'z') {
					chars[pos] = next_char[chars[pos]];
					break;
				}
				pos -= 1;
				if (pos == -1) {
					chars.unshift('a');
					numChars += 1;
					break;
				}
			}
			if (pos != numChars-1) {
				for (var j = pos+1; j <= numChars-1; ++j) {
					chars[j] = 'a';
				}
			}
		}
	});

	// permute randomly
	var shuffledStrings = sortedStrings.slice();
	timeFunction('shuffled', function () {
		shuffle(shuffledStrings);
	});

	// sort string arrays
	testQuicksort(shuffledStrings.slice());
	testNativeSort(shuffledStrings.slice());

	// object sorting
	print('objects:');

	// make student objects based on the sorted strings
	var sortedStudents = new Array(n);
	timeFunction('made student objects', function () {
		for (var i = 0; i < n; ++i) {
			sortedStudents[i] = { name: sortedStrings[i], grade: i%10 };
		}
	});

	// permute randomly
	var shuffledStudents = sortedStudents.slice();
	timeFunction('shuffled', function () {
		shuffle(shuffledStudents);
	});

	// sort objects
	function compareStudents(a, b) {
		if (a.grade != b.grade) {
			return a.grade - b.grade;
		} else if (a.name < b.name) {
			return -1;
		} else if (a.name > b.name) {
			return 1;
		}
		return 0;
	}
	testQuicksortCustom(shuffledStudents.slice(), compareStudents);
	testNativeSort(shuffledStudents.slice(), compareStudents);
}

main();
