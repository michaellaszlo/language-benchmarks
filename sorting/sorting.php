#!/usr/bin/php5
<?php

//--- heapsort: in-place implementation with linear-time heap construction

function trickleDown(&$data, $pos, $n) {
	$left = 2*$pos+1;
	if ($left >= $n) {
		return;
	}
	$right = 2*$pos+2;
	if ($right >= $n) {
		if ($data[$pos] < $data[$left]) {
			$t = $data[$pos];
			$data[$pos] = $data[$left];
			$data[$left] = $t;
		}
		return;
	}
	if ($data[$pos] < $data[$left]) {
		if ($data[$left] < $data[$right]) {
			$t = $data[$pos];
			$data[$pos] = $data[$right];
			$data[$right] = $t;
			trickleDown($data, $right, $n);
		} else {
			$t = $data[$pos];
			$data[$pos] = $data[$left];
			$data[$left] = $t;
			trickleDown($data, $left, $n);
		}
	} else if ($data[$pos] < $data[$right]) {
		$t = $data[$pos];
		$data[$pos] = $data[$right];
		$data[$right] = $t;
		trickleDown($data, $right, $n);
	}
}

function heapsort(&$data) {
	# build a max-heap
	$n = count($data);
	for ($i = (int)(($n-2)/2); $i != -1; --$i) {
		trickleDown($data, $i, $n);
	}

	# successively remove the max item and swap it to the end
	for ($i = $n-1; $i != -1; --$i) {
		$t = $data[$i];
		$data[$i] = $data[0];
		$data[0] = $t;
		trickleDown($data, 0, $i);
	}
}

function testHeapsort($data) {
	timeFunction("ran heapsort", function () use (&$data) {
		heapsort($data);
	});
	if (!isSorted($data)) {
		printf("error: data is not sorted\n");
	}
}


//--- mergesort: recursive implementation with two arrays

function mergesort(&$data, &$buffer, $left, $right) {
	if ($left+1 >= $right) {
		return;
	}
	$mid = (int)(($left+$right)/2);
	mergesort($data, $buffer, $left, $mid);
	mergesort($data, $buffer, $mid, $right);
	$pos_left = $left;
	$pos_right = $mid;
	$pos_buffer = $left;
	while (true) {
		if ($data[$pos_left] <= $data[$pos_right]) {
			$buffer[$pos_buffer] = $data[$pos_left];
			$pos_buffer += 1;
			$pos_left += 1;
			if ($pos_left == $mid) {
				while ($pos_right != $right) {
					$buffer[$pos_buffer] = $data[$pos_right];
					++$pos_buffer;
					++$pos_right;
				}
				break;
			}
		} else {
			$buffer[$pos_buffer] = $data[$pos_right];
			$pos_buffer += 1;
			$pos_right += 1;
			if ($pos_right == $right) {
				while ($pos_left != $mid) {
					$buffer[$pos_buffer] = $data[$pos_left];
					++$pos_buffer;
					++$pos_left;
				}
				break;
			}
		}
	}
	for ($i = $left; $i < $right; ++$i) {
		$data[$i] = $buffer[$i];
	}
}

function testMergesort($data) {
	timeFunction("ran mergesort", function () use (&$data) {
		$buffer = [];
		mergesort($data, $buffer, 0, count($data));
	});
	if (!isSorted($data)) {
		printf("error: data is not sorted\n");
	}
}


//--- quicksort: in-place implementation with median-of-three pivot selection

function quicksort(&$data, $left, $right) {
	if ($left+1 >= $right) {
		return;
	}
	$ai = $left;
	$bi = (int)(($left+$right)/2);
	$ci = $right-1;
	$a = $data[$ai];
	$b = $data[$bi];
	$c = $data[$ci];
    if ($a < $b) {
        if ($c < $a) {
            $pos = $ai;
		} else if ($c < $b) {
			$pos = $ci;
		} else {
			$pos = $bi;
		}
	} else {
        if ($c < $b) {
            $pos = $bi;
		} else if ($c < $a) {
			$pos = $ci;
		} else {
			$pos = $ai;
		}
	}
    $pivot = $data[$pos];
	$data[$pos] = $data[$right-1];
	$tail = $left;
	for ($i = $left; $i != $right-1; ++$i) {
		if ($data[$i] < $pivot) {
			$t = $data[$tail];
			$data[$tail] = $data[$i];
			$data[$i] = $t;
			++$tail;
		}
	}
	$data[$right-1] = $data[$tail];
	$data[$tail] = $pivot;
    quicksort($data, $left, $tail);
    quicksort($data, $tail+1, $right);
}

function testQuicksort($data) {
	timeFunction("ran quicksort", function () use (&$data) {
		quicksort($data, 0, count($data));
	});
	if (!isSorted($data)) {
		printf("error: data is not sorted\n");
	}
}

function quicksortCustom(&$data, $left, $right, $compare) {
	if ($left+1 >= $right) {
		return;
	}
	$ai = $left;
	$bi = (int)(($left+$right)/2);
	$ci = $right-1;
	$a = $data[$ai];
	$b = $data[$bi];
	$c = $data[$ci];
    if ($compare($a, $b) == -1) {
        if ($compare($c, $a) == -1) {
            $pos = $ai;
		} else if ($compare($c, $b) == -1) {
			$pos = $ci;
		} else {
			$pos = $bi;
		}
	} else {
        if ($compare($c, $b) == -1) {
            $pos = $bi;
		} else if ($compare($c, $a) == -1) {
			$pos = $ci;
		} else {
			$pos = $ai;
		}
	}
    $pivot = $data[$pos];
	$data[$pos] = $data[$right-1];
	$tail = $left;
	for ($i = $left; $i != $right-1; ++$i) {
		if ($compare($data[$i], $pivot) == -1) {
			$t = $data[$tail];
			$data[$tail] = $data[$i];
			$data[$i] = $t;
			++$tail;
		}
	}
	$data[$right-1] = $data[$tail];
	$data[$tail] = $pivot;
    quicksortCustom($data, $left, $tail, $compare);
    quicksortCustom($data, $tail+1, $right, $compare);
}

function testQuicksortCustom($data, $compare) {
	timeFunction("ran quicksort", function () use (&$data, $compare) {
		quicksortCustom($data, 0, count($data), $compare);
	});
	if (!isSortedCustom($data, $compare)) {
		printf("error: data is not sorted\n");
	}
}


//--- native sort

function testNativeSortInteger($data) {
	timeFunction("ran native sort", function () use (&$data) {
		sort($data, SORT_NUMERIC);
	});
	if (!isSorted($data)) {
		printf("error: data is not sorted\n");
	}
}

function testNativeSortString($data) {
	timeFunction("ran native sort", function () use (&$data) {
		sort($data, SORT_STRING);
	});
	if (!isSorted($data)) {
		printf("error: data is not sorted\n");
	}
}

function testNativeSortCustom($data, $compare) {
	timeFunction("ran native sort", function () use (&$data, $compare) {
		usort($data, $compare);
	});
	if (!isSortedCustom($data, $compare)) {
		printf("error: data is not sorted\n");
	}
}


//--- random permutation

function shuffleData(&$data) {
    // constants for a linear congruential generator
    $m = (1 << 32);
	$a = 1664525;
	$c = 1013904223;
	$randomValue = 42;  // seed
    for ($i = count($data)-1; $i != 0; --$i) {
        $randomValue = ($a*$randomValue + $c) % $m;
        $j = $randomValue % ($i+1);
        if ($j != $i) {
            $t = $data[$i];
            $data[$i] = $data[$j];
            $data[$j] = $t;
        }
    }
}


//--- helper functions

function timeFunction($description, $function) {
	$start_time = microtime(true);
	$function();
	$elapsed = microtime(true) - $start_time;
	printf("    %s  %.3f s\n", $description, $elapsed);
}

function isSorted(&$arr) {
	$n = count($arr);
	for ($i = 1; $i < $n; ++$i) {
		if ($arr[$i-1] > $arr) {
			return false;
		}
	}
	return true;
}

function isSortedCustom(&$arr, $compare) {
	$n = count($arr);
	for ($i = 1; $i < $n; ++$i) {
		if ($compare($arr[$i-1], $arr[$i]) == 1) {
			return false;
		}
	}
	return true;
}

function display(&$arr) {
	echo $arr[0];
	$n = count($arr);
	for ($i = 1; $i < $n; ++$i) {
		echo " ".$arr[$i];
	}
	echo "\n";
}

function displayStudents(&$arr) {
	echo implode(" ", [$arr[0]->grade, $arr[0]->name]);
	$n = count($arr);
	for ($i = 1; $i < $n; ++$i) {
		echo " ".implode(" ", [$arr[$i]->grade, $arr[$i]->name]);
	}
	echo "\n";
}


function main() {
	global $argc, $argv;
	if ($argc != 2) {
		exit("error: must specify the length of the array\n");
	}
	$n = intval($argv[1]);


	//--- integer sorting
	echo "integers:\n";

    // initialize an array with 0, ..., n-1
	$sorted_integers = null;
	timeFunction("made ordered integer array", function ()
			use (&$sorted_integers, $n) {
		$sorted_integers = [];
		for ($i = 0; $i < $n; ++$i) {
			$sorted_integers[] = $i;
		}
	});

	// permute randomly
	$shuffled_integers = $sorted_integers;
	timeFunction("shuffled", function () use (&$shuffled_integers) {
		shuffleData($shuffled_integers);
	});

	// sort integer arrays
	testHeapsort($shuffled_integers);
	testMergesort($shuffled_integers);
	testQuicksort($shuffled_integers);
	testNativeSortInteger($shuffled_integers);


	//--- string sorting
	echo "strings:\n";
	
    // make strings: 'a', 'b', ..., 'z', 'aa', 'ab', ..., 'az', 'ba', ...
	$sorted_strings = null;
	timeFunction("made ordered string array", function ()
			use (&$sorted_strings, $n) {
		$sorted_strings = [];
		$next_char = [];
		$zCode = ord('z');
		for ($code = ord('a'); $code != $zCode; ++$code) {
			$next_char[chr($code)] = chr($code+1);
		}
		$chars = ['a'];
		$num_chars = 1;
		for ($i = 0; $i != $n; ++$i) {
			$sorted_strings[] = implode($chars);
			$pos = $num_chars-1;
			while (true) {
				if ($chars[$pos] != 'z') {
					$chars[$pos] = $next_char[$chars[$pos]];
					break;
				}
				$pos -= 1;
				if ($pos == -1) {
					array_unshift($chars, 'a');
					$num_chars += 1;
					break;
				}
			}
			if ($pos != $num_chars-1) {
				for ($j = $pos+1; $j <= $num_chars-1; ++$j) {
					$chars[$j] = 'a';
				}
			}
		}
	});

    // permute randomly
	$shuffled_strings = $sorted_strings;
	timeFunction("shuffled", function () use (&$shuffled_strings) {
		shuffleData($shuffled_strings);
	});

    // sort string arrays
    testQuicksort($shuffled_strings);
    testNativeSortString($shuffled_strings);


	//--- object sorting
	echo "objects:\n";

	class Student {
		function Student($name, $grade) {
			$this->name = $name;
			$this->grade = $grade;
		}
	}

	// make student objects based on the sorted strings
	$sorted_students = null;
	timeFunction("made student objects", function ()
			use (&$sorted_students, &$sorted_strings, $n) {
		$sorted_students = [];
		for ($i = 0; $i < $n; ++$i) {
			$sorted_students[] = new Student($sorted_strings[$i], $i%10);
		}
	});

    // permute randomly
    $shuffled_students = $sorted_students;
	timeFunction("shuffled", function () use (&$shuffled_students) {
		shuffle($shuffled_students);
	});

	// sort objects
	$compareStudents = function ($a, $b) {
		if ($a->grade < $b->grade) {
			return -1;
		} else if ($a->grade > $b->grade) {
			return 1;
		} else if ($a->name < $b->name) {
			return -1;
		} else if ($a->name > $b->name) {
			return 1;
		}
		return 0;
	};
    testQuicksortCustom($shuffled_students, $compareStudents);
    testNativeSortCustom($shuffled_students, $compareStudents);
}

main();
