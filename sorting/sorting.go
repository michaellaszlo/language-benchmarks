package main

import (
	"os"
	"fmt"
	"strconv"
	"time"
	"sort"
)


//--- heapsort: in-place implementation with linear-time heap construction

func trickleDown(data []int, pos, n int) {
	left := 2*pos+1
	if (left >= n) {
		return
	}
	right := 2*pos+2
	if (right >= n) {
		if (data[pos] < data[left]) {
			data[pos], data[left] = data[left], data[pos]
		}
		return;
	}
	if (data[pos] < data[left]) {
		if (data[left] < data[right]) {
			data[pos], data[right] = data[right], data[pos]
			trickleDown(data, right, n)
		} else {
			data[pos], data[left] = data[left], data[pos]
			trickleDown(data, left, n)
		}
	} else if(data[pos] < data[right]) {
		data[pos], data[right] = data[right], data[pos]
		trickleDown(data, right, n)
	}
}

func heapsort(data []int) {
	// build a max-heap
	n := len(data)
	for i := (n-2)/2; i != -1; i-- {
		trickleDown(data, i, n)
	}

	// successively remove the max item and swap it to the end
	for i := n-1; i != -1; i-- {
		data[i], data[0] = data[0], data[i]
		trickleDown(data, 0, i)
	}
}

func testHeapsort(data []int) {
	timeFunction("ran heapsort", func() {
		heapsort(data);
	})
	if !sort.IntsAreSorted(data) {
		fmt.Printf("error: data is not sorted\n")
	}
}


//--- mergesort: recursive implementation with two arrays

func mergesort(data, buffer []int, left, right int) {
	if left+1 == right {
		return
	}
	mid := (left+right)/2
	mergesort(data, buffer, left, mid)
	mergesort(data, buffer, mid, right)
	posLeft, posRight := left, mid
	posBuffer := left
	for {
		if data[posLeft] <= data[posRight] {
			buffer[posBuffer] = data[posLeft]
			posBuffer++
			posLeft++
			if posLeft == mid {
				for posRight != right {
					buffer[posBuffer] = data[posRight]
					posBuffer++
					posRight++
				}
				break
			}
		} else {
			buffer[posBuffer] = data[posRight]
			posBuffer++
			posRight++
			if posRight == right {
				for posLeft != mid {
					buffer[posBuffer] = data[posLeft]
					posBuffer++
					posLeft++
				}
				break
			}
		}
	}
	for i := left; i != right; i++ {
		data[i] = buffer[i]
	}
}

func testMergesort(data []int) {
	timeFunction("ran mergesort", func() {
		buffer := make([]int, len(data))
		mergesort(data, buffer, 0, len(data))
	})
	if !sort.IntsAreSorted(data) {
		fmt.Printf("error: data is not sorted\n")
	}
}


//--- quicksort: in-place implementation with median-of-three pivot selection

func quicksortInt(data []int, left, right int) {
	if (left+1 >= right) {
		return
	}
	ai, bi, ci := left, (left+right)/2, right-1
	a, b, c := data[ai], data[bi], data[ci]
	var pos int
    if (a < b) {
        if (c < a) {
			pos = ai
		} else if (c < b) {
			pos = ci
		} else {
			pos = bi
		}
	} else {
        if (c < b) {
			pos = bi
		} else if (c < a) {
			pos = ci
		} else {
			pos = ai
		}
	}
	pivot := data[pos]
	data[pos] = data[right-1]
	tail := left
	for i := left; i != right-1; i++ {
		if (data[i] < pivot) {
			data[tail], data[i] = data[i], data[tail]
			tail++
		}
	}
	data[right-1] = data[tail]
	data[tail] = pivot
    quicksortInt(data, left, tail)
    quicksortInt(data, tail+1, right)
}

func testQuicksortInt(data []int) {
	timeFunction("ran quicksort", func() {
		quicksortInt(data, 0, len(data))
	})
	if !sort.IntsAreSorted(data) {
		fmt.Printf("error: data is not sorted\n")
	}
}

func quicksortString(data []string, left, right int) {
	if (left+1 >= right) {
		return
	}
	ai, bi, ci := left, (left+right)/2, right-1
	a, b, c := data[ai], data[bi], data[ci]
	var pos int
    if (a < b) {
        if (c < a) {
			pos = ai
		} else if (c < b) {
			pos = ci
		} else {
			pos = bi
		}
	} else {
        if (c < b) {
			pos = bi
		} else if (c < a) {
			pos = ci
		} else {
			pos = ai
		}
	}
	pivot := data[pos]
	data[pos] = data[right-1]
	tail := left
	for i := left; i != right-1; i++ {
		if (data[i] < pivot) {
			data[tail], data[i] = data[i], data[tail]
			tail++
		}
	}
	data[right-1] = data[tail]
	data[tail] = pivot
    quicksortString(data, left, tail)
    quicksortString(data, tail+1, right)
}

func testQuicksortString(data []string) {
	timeFunction("ran quicksort", func() {
		quicksortString(data, 0, len(data))
	})
	if !sort.StringsAreSorted(data) {
		fmt.Printf("error: data is not sorted\n")
	}
}

func quicksortStudent(data Students, left, right int) {
	if (left+1 >= right) {
		return
	}
	ai, bi, ci := left, (left+right)/2, right-1
	var pos int
    if (data.Less(ai, bi)) {
        if (data.Less(ci, ai)) {
			pos = ai
		} else if (data.Less(ci, bi)) {
			pos = ci
		} else {
			pos = bi
		}
	} else {
        if (data.Less(ci, bi)) {
			pos = bi
		} else if (data.Less(ci, ai)) {
			pos = ci
		} else {
			pos = ai
		}
	}
	pivot := data[pos]
	data[pos] = data[right-1]
	tail := left
	for i := left; i != right-1; i++ {
		if (data[i].LessThan(pivot)) {
			data[tail], data[i] = data[i], data[tail]
			tail++
		}
	}
	data[right-1] = data[tail]
	data[tail] = pivot
    quicksortStudent(data, left, tail)
    quicksortStudent(data, tail+1, right)
}

func testQuicksortStudent(data Students) {
	timeFunction("ran quicksort", func() {
		quicksortStudent(data, 0, len(data))
	})
	if !sort.IsSorted(sort.Interface(data)) {
		fmt.Printf("error: data is not sorted\n")
	}
}


//--- native sort

// for ints
func testNativeSortInteger(data []int) {
	timeFunction("ran native sort", func() {
		sort.Ints(data);
	})
	if !sort.IntsAreSorted(data) {
		fmt.Printf("error: data is not sorted\n")
	}
}

// for strings
func testNativeSortString(data []string) {
	timeFunction("ran native sort", func() {
		sort.Strings(data);
	})
	if !sort.StringsAreSorted(data) {
		fmt.Printf("error: data is not sorted\n")
	}
}

// for anything that implements sort.Interface
func testNativeSort(data sort.Interface) {
	timeFunction("ran native sort", func() {
		sort.Sort(data);
	})
	if !sort.IsSorted(data) {
		fmt.Printf("error: data is not sorted\n")
	}
}


//--- student objects

type Student struct {
	Name string
	Grade int
}
type Students []Student

func (a Student) LessThan(b Student) bool {
	if a.Grade != b.Grade {
		return a.Grade < b.Grade
	}
	return a.Name < b.Name
}

// methods for sort.Interface
func (students Students) Len() int {
	return len(students)
}
func (students Students) Less(i, j int) bool {
	if students[i].Grade != students[j].Grade {
		return students[i].Grade < students[j].Grade
	}
	return students[i].Name < students[j].Name
}
func (students Students) Swap(i, j int) {
	students[i], students[j] = students[j], students[i]
}


//--- random permutation

func shuffle(data sort.Interface) {
    // constants for a linear congruential generator
	m := int64(1<<32)
	a := int64(1664525)
	c := int64(1013904223)
	randomValue := int64(42)  // seed
	for i := data.Len()-1; i != 0; i-- {
        randomValue = (a*randomValue + c) % m
		j := int(randomValue) % (i+1)
        if j != i {
            data.Swap(i, j)
        }
    }
}


//--- helper functions

func timeFunction(description string, fun func()) {
	startTime := time.Now()
	fun()
	elapsed := float64(time.Since(startTime))/1e9
	fmt.Printf("    %s  %.3f s\n", description, elapsed)
}

func displaySliceInt(data []int) {
	fmt.Printf("%d", data[0])
	for i := 1; i != len(data); i++ {
		fmt.Printf(" %d", data[i])
	}
	fmt.Printf("\n")
}

func displaySliceString(data []string) {
	fmt.Printf("%s", data[0])
	for i := 1; i != len(data); i++ {
		fmt.Printf(" %s", data[i])
	}
	fmt.Printf("\n")
}

func displaySliceStudent(data []Student) {
	fmt.Printf("%d %s", data[0].Grade, data[0].Name)
	for i := 1; i != len(data); i++ {
		fmt.Printf(" %d %s", data[i].Grade, data[i].Name)
	}
	fmt.Printf("\n")
}


func main() {
	if len(os.Args) != 2 {
		fmt.Printf("error: must specify the length of the array\n")
		return
	}
	n, _ := strconv.Atoi(os.Args[1])


	//--- integer sorting
	fmt.Println("integers:")

	// initialize an array with 0, ..., n-1
	var sortedIntegers []int
	timeFunction("made ordered integer array", func() {
		sortedIntegers = make([]int, n)
		for i := range sortedIntegers {
			sortedIntegers[i] = i
		}
	})
	//elapsed := float64(time.Since(startTime))/1e9
	//fmt.Printf("    made ordered integer array in %.3f s\n", elapsed)

	// permute randomly
	shuffledIntegers := make([]int, n)
	copy(shuffledIntegers, sortedIntegers)
	timeFunction("shuffled", func() {
		shuffle(sort.IntSlice(shuffledIntegers))
	})

	// sort integer arrays
	intData := make([]int, n)
	copy(intData, shuffledIntegers)
	testHeapsort(intData);
	copy(intData, shuffledIntegers)
	testMergesort(intData);
	copy(intData, shuffledIntegers)
	testQuicksortInt(intData);
	copy(intData, shuffledIntegers)
	testNativeSortInteger(intData);


	//--- string sorting
	fmt.Println("strings:")

    // make strings: 'a', 'b', ..., 'z', 'aa', 'ab', ..., 'az', 'ba', ...
	var sortedStrings []string
	timeFunction("made ordered string array", func() {
		sortedStrings = make([]string, n)
		chars := []byte{'a'}
		numChars := 1
		for i := 0; i != n; i++ {
			sortedStrings[i] = string(chars)
			pos := numChars-1
			for {
				if chars[pos] != 'z' {
					chars[pos]++
					break
				}
				pos -= 1
				if pos == -1 {
					chars = append([]byte{'a'}, chars...)
					numChars += 1
					break
				}
			}
			if pos != numChars-1 {
				for j := pos+1; j <= numChars-1; j++ {
					chars[j] = 'a'
				}
			}
		}
	})

    // permute randomly
	shuffledStrings := make([]string, n)
	copy(shuffledStrings, sortedStrings)
	timeFunction("shuffled", func() {
		shuffle(sort.StringSlice(shuffledStrings))
	})

	// sort string arrays
	stringData := make([]string, n)
	copy(stringData, shuffledStrings)
	testQuicksortString(stringData)
	copy(stringData, shuffledStrings)
	testNativeSortString(stringData)


    //--- object sorting
	fmt.Println("objects:")

    // make student objects based on the sorted strings
	var sortedStudents []Student
	timeFunction("made student objects", func() {
		sortedStudents = make([]Student, n)
		for i, s := range sortedStrings {
			sortedStudents[i] = Student{s, i%10}
		}
	})

    // permute randomly
	shuffledStudents := make([]Student, n)
	copy(shuffledStudents, sortedStudents)
	timeFunction("shuffled", func() {
		shuffle(Students(shuffledStudents))
	})

    // sort objects
	data := make([]Student, n)
	copy(data, shuffledStudents)
	testQuicksortStudent(Students(data))
	copy(data, shuffledStudents)
	testNativeSort(Students(data))

	/*
	timeFunction("ran native sort", func() {
		testQuicksort()
	})

    # extra test with sorting on keys
    data = shuffled_students[0..-1]
    time_block 'sorted natively with keys' do
        data.sort_by! {|student| [student.grade, student.name] }
    end
	*/
}
