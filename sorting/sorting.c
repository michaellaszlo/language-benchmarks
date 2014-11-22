#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct timespec start_time, stop_time;
void displayElapsed(const char *s);

typedef struct {
	char *name;
	int grade;
} student;

student *new_student(char *name, int grade) {
	student *new = (student *) malloc(sizeof(student));
	new->name = name;
	new->grade = grade;
	return new;
}

void displayInt(int *data, int n);
int isSortedInt(int *data, int n);
void displayString(char **data, int n);
int isSortedString(char **data, int n);
void displayStudent(student **data, int n);
int isSortedStudent(student **data, int n);
int compareStudent(const void *a, const void *b);


/*--- heapsort: in-place implementation with linear-time heap construction */

void trickle_down(int *data, int pos, int n) {
	int left = 2*pos+1, right, t;
	if (left >= n) {
		return;
	}
	right = 2*pos+2;
	if (right >= n) {
		if (data[pos] < data[left]) {
			t = data[pos];
			data[pos] = data[left];
			data[left] = t;
		}
		return;
	}
	if (data[pos] < data[left]) {
		if (data[left] < data[right]) {
			t = data[pos];
			data[pos] = data[right];
			data[right] = t;
			trickle_down(data, right, n);
		} else {
			t = data[pos];
			data[pos] = data[left];
			data[left] = t;
			trickle_down(data, left, n);
		}
	} else if(data[pos] < data[right]) {
		t = data[pos];
		data[pos] = data[right];
		data[right] = t;
		trickle_down(data, right, n);
	}
}

void heapsort(int *data, int n) {
	int i, t;
	/* build a max-heap */
	for (i = (n-2)/2; i != -1; --i) {
		trickle_down(data, i, n);
	}

	/* successively remove the max item and swap it to the end */
	for (i = n-1; i != -1; --i) {
		t = data[i];
		data[i] = data[0];
		data[0] = t;
		trickle_down(data, 0, i);
	}
}

void testHeapsort(int *data, int n) {
	clock_gettime(CLOCK_REALTIME, &start_time);
	heapsort(data, n);
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    ran heapsort");
	if (!isSortedInt(data, n)) {
		printf("error: data is not sorted\n");
	}
}


/*--- mergesort: recursive implementation with two arrays */

void mergesort(int *data, int *buffer, int left, int right) {
	int mid, posLeft, posRight, posBuffer, i;
	if (left+1 == right) {
		return;
	}
	mid = (left+right)/2;
	mergesort(data, buffer, left, mid);
	mergesort(data, buffer, mid, right);
	posLeft = left;
	posRight = mid;
	posBuffer = left;
	while (1) {
		if (data[posLeft] <= data[posRight]) {
			buffer[posBuffer++] = data[posLeft++];
			if (posLeft == mid) {
				while (posRight != right) {
					buffer[posBuffer++] = data[posRight++];
				}
				break;
			}
		} else {
			buffer[posBuffer++] = data[posRight++];
			if (posRight == right) {
				while (posLeft != mid) {
					buffer[posBuffer++] = data[posLeft++];
				}
				break;
			}
		}
	}
	for (i = left; i != right; ++i) {
		data[i] = buffer[i];
	}
}

void testMergesort(int *data, int n) {
	int *buffer;
	clock_gettime(CLOCK_REALTIME, &start_time);
	buffer = (int *) malloc(n*sizeof(int)); 
	mergesort(data, buffer, 0, n);
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    ran mergesort");
	if (!isSortedInt(data, n)) {
		printf("error: data is not sorted\n");
	}
}


/*--- quicksort: in-place implementation with median-of-three pivot selection */

void quicksort(int *data, int left, int right) {
	int ai, bi, ci, a, b, c, pos, pivot, tail, i, t;
	if (left+1 >= right) {
		return;
	}
	ai = left;
	bi = (left+right)/2;
	ci = right-1;
	a = data[ai];
	b = data[bi];
	c = data[ci];
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
    pivot = data[pos];
	data[pos] = data[right-1];
	tail = left;
	for (i = left; i != right-1; ++i) {
		if (data[i] < pivot) {
			t = data[tail];
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

void testQuicksort(int *data, int n) {
	clock_gettime(CLOCK_REALTIME, &start_time);
	quicksort(data, 0, n);
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    ran quicksort");
	if (!isSortedInt(data, n)) {
		printf("error: data is not sorted\n");
	}
}

void quicksortString(char **data, int left, int right) {
	int ai, bi, ci, pos, tail, i;
	char *a, *b, *c, *pivot, *t;
	if (left+1 >= right) {
		return;
	}
	ai = left;
	bi = (left+right)/2;
	ci = right-1;
	a = data[ai];
	b = data[bi];
	c = data[ci];
    if (strcmp(a, b) < 0) {
        if (strcmp(c, a) < 0) {
            pos = ai;
		} else if (strcmp(c, b) < 0) {
			pos = ci;
		} else {
			pos = bi;
		}
	} else {
        if (strcmp(c, b) < 0) {
            pos = bi;
		} else if (strcmp(c, a) < 0) {
			pos = ci;
		} else {
			pos = ai;
		}
	}
    pivot = data[pos];
	data[pos] = data[right-1];
	tail = left;
	for (i = left; i != right-1; ++i) {
		if (strcmp(data[i], pivot) < 0) {
			t = data[tail];
			data[tail] = data[i];
			data[i] = t;
			++tail;
		}
	}
	data[right-1] = data[tail];
	data[tail] = pivot;
    quicksortString(data, left, tail);
    quicksortString(data, tail+1, right);
}

void testQuicksortString(char **data, int n) {
	clock_gettime(CLOCK_REALTIME, &start_time);
	quicksortString(data, 0, n);
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    ran quicksort");
	if (!isSortedString(data, n)) {
		printf("error: data is not sorted\n");
	}
}

void quicksortStudent(student **data, int left, int right) {
	int ai, bi, ci, pos, tail, i;
	student *a, *b, *c, *pivot, *t;
	if (left+1 >= right) {
		return;
	}
	ai = left;
	bi = (left+right)/2;
	ci = right-1;
	a = data[ai];
	b = data[bi];
	c = data[ci];
    if (compareStudent(a, b) < 0) {
        if (compareStudent(c, a) < 0) {
            pos = ai;
		} else if (compareStudent(c, b) < 0) {
			pos = ci;
		} else {
			pos = bi;
		}
	} else {
        if (compareStudent(c, b) < 0) {
            pos = bi;
		} else if (compareStudent(c, a) < 0) {
			pos = ci;
		} else {
			pos = ai;
		}
	}
    pivot = data[pos];
	data[pos] = data[right-1];
	tail = left;
	for (i = left; i != right-1; ++i) {
		if (compareStudent(data[i], pivot) < 0) {
			t = data[tail];
			data[tail] = data[i];
			data[i] = t;
			++tail;
		}
	}
	data[right-1] = data[tail];
	data[tail] = pivot;
    quicksortStudent(data, left, tail);
    quicksortStudent(data, tail+1, right);
}

void testQuicksortStudent(student **data, int n) {
	clock_gettime(CLOCK_REALTIME, &start_time);
	quicksortStudent(data, 0, n);
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    ran quicksort");
	if (!isSortedStudent(data, n)) {
		printf("error: data is not sorted\n");
	}
}


/*--- native sort on integers */

int compareInt(const void *a, const void *b) {
	return *(int *)a - *(int *)b;
}

void testNativeSortInt(int *data, int n) {
	clock_gettime(CLOCK_REALTIME, &start_time);
	qsort(data, n, sizeof(int), compareInt);
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    ran native sort");
	if (!isSortedInt(data, n)) {
		printf("error: data is not sorted\n");
	}
}


/*--- native sort on strings */

int compareString(const void *a, const void *b) {
	return strcmp(*(char **)a, *(char **)b);
}

void testNativeSortString(char **data, int n) {
	clock_gettime(CLOCK_REALTIME, &start_time);
	qsort(data, n, sizeof(char *), compareString);
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    ran native sort");
	if (!isSortedString(data, n)) {
		printf("error: data is not sorted\n");
	}
}


/*--- native sort on students */

int compareStudent(const void *a, const void *b) {
	student *as = (student *)a, *bs = (student *)b;
	if (as->grade != bs->grade) {
		return as->grade - bs->grade;
	}
	return strcmp(as->name, bs->name);
}
int compareStudentPointer(const void *a, const void *b) {
	return compareStudent(*(student **)a, *(student **)b);
}

void testNativeSortStudent(student **data, int n) {
	clock_gettime(CLOCK_REALTIME, &start_time);
	qsort(data, n, sizeof(char *), compareStudentPointer);
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    ran native sort");
	if (!isSortedStudent(data, n)) {
		printf("error: data is not sorted\n");
	}
}


/*--- random permutation */

void shuffleInt(int *data, int n) {
	int i, j, t;
	/* constants for a linear congruential generator */
	long long m = (1L << 32),
		 a = 1664525,
		 c = 1013904223,
		 randomValue = 42;  /* seed */
    for (i = n-1; i != 0; --i) {
        randomValue = (a*randomValue + c) % m;
        j = randomValue % (i+1);
        if (j != i) {
            t = data[i];
            data[i] = data[j];
            data[j] = t;
        }
    }
}

void shuffle(void **data, int n) {
	int i, j;
	char *t;
	/* constants for a linear congruential generator */
	long long m = (1L << 32),
		 a = 1664525,
		 c = 1013904223,
		 randomValue = 42;  /* seed */
    for (i = n-1; i != 0; --i) {
        randomValue = (a*randomValue + c) % m;
        j = randomValue % (i+1);
        if (j != i) {
            t = data[i];
            data[i] = data[j];
            data[j] = t;
        }
    }
}


/*--- helper functions */

void displayInt(int *data, int n) {
	int i;
	printf("%d", data[0]);
	for (i = 1; i < n; ++i) {
		printf(" %d", data[i]);
	}
	printf("\n");
}

void displayString(char **data, int n) {
	int i;
	printf("%s", data[0]);
	for (i = 1; i < n; ++i) {
		printf(" %s", data[i]);
	}
	printf("\n");
}

void displayStudent(student **students, int n) {
	int i;
	printf("%d %s", students[0]->grade, students[0]->name);
	for (i = 1; i < n; ++i) {
		printf(" %d %s", students[i]->grade, students[i]->name);
	}
	printf("\n");
}

int isSortedInt(int *data, int n) {
	int i;
	for (i = 1; i < n; ++i) {
		if (data[i-1] > data[i]) {
			return 0;
		}
	}
	return 1;
}

int isSortedString(char **data, int n) {
	int i;
	for (i = 1; i < n; ++i) {
		if (strcmp(data[i-1], data[i]) > 0) {
			return 0;
		}
	}
	return 1;
}

int isSortedStudent(student **data, int n) {
	int i;
	for (i = 1; i < n; ++i) {
		if (compareStudent(data[i-1], data[i]) > 0) {
			printf("%d %s > %d %s\n", data[i-1]->grade, data[i-1]->name,
					data[i]->grade, data[i]->name);
			return 0;
		}
	}
	return 1;
}

void displayElapsed(const char *s) {
	double elapsed = stop_time.tv_sec - start_time.tv_sec;
	elapsed += ((double)stop_time.tv_nsec - start_time.tv_nsec)/1000000000;
	printf("%s in %.3f s\n", s, elapsed);
}


int main(int arg_num, char **args) {
	int n, *sorted_integers, *shuffled_integers, *data_int,
		num_chars, string_size, i, j, pos;
	char **sorted_strings, **shuffled_strings, **data_string,
		 *chars, *old_chars;
	student **sorted_students, **shuffled_students, **data_student;

	if (arg_num != 2) {
		printf("error: must specify the length of the array\n");
		return 0;
	}
	n = atoi(args[1]);


	/*--- integer sorting */
	printf("integers:\n");

	/* initialize an array with 0, ..., n-1 */
	clock_gettime(CLOCK_REALTIME, &start_time);
	sorted_integers = (int *) malloc(n*sizeof(int));
	for (i = 0; i < n; ++i) {
		sorted_integers[i] = i;
	}
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    made ordered integer array");

	/* permute randomly */
	shuffled_integers = (int *) malloc(n*sizeof(int));
	memcpy(shuffled_integers, sorted_integers, n*sizeof(int));
	clock_gettime(CLOCK_REALTIME, &start_time);
	shuffleInt(shuffled_integers, n);
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    permuted randomly");

	/* sort integer arrays */
	data_int = (int *) malloc(n*sizeof(int));
	memcpy(data_int, shuffled_integers, n*sizeof(int));
	testHeapsort(data_int, n);
	memcpy(data_int, shuffled_integers, n*sizeof(int));
	testMergesort(data_int, n);
	memcpy(data_int, shuffled_integers, n*sizeof(int));
	testQuicksort(data_int, n);
	memcpy(data_int, shuffled_integers, n*sizeof(int));
	testNativeSortInt(data_int, n);


	/*--- string sorting */
	printf("strings:\n");

	clock_gettime(CLOCK_REALTIME, &start_time);
	sorted_strings = (char **) malloc(n*sizeof(char *));
	chars = (char *) malloc(2*sizeof(char *));
	sprintf(chars, "a");
    num_chars = 2;
	string_size = num_chars*sizeof(char);
    for (i = 0; i != n; ++i) {
        sorted_strings[i] = (char *) malloc(num_chars*sizeof(char *));
		memcpy(sorted_strings[i], chars, string_size);
        pos = num_chars-2;
        while (1) {
            if (chars[pos] != 'z') {
                ++chars[pos];
                break;
            }
            --pos;
            if (pos == -1) {
				old_chars = chars;
				++num_chars;
				chars = (char *) malloc(string_size+sizeof(char));
				chars[0] = 'a';
				memcpy(&chars[1], old_chars, string_size);
				string_size += sizeof(char);
				free(old_chars);
                break;
            }
        }
        if (pos != num_chars-2) {
            for (j = pos+1; j <= num_chars-2; ++j) {
                chars[j] = 'a';
            }
        }
    }
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    made ordered string array");

	/* permute randomly */
	shuffled_strings = (char **) malloc(n*sizeof(char *));
	memcpy(shuffled_strings, sorted_strings, n*sizeof(char *));
	clock_gettime(CLOCK_REALTIME, &start_time);
	shuffle((void **)shuffled_strings, n);
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    permuted randomly");

	/* sort string arrays */
	data_string = (char **) malloc(n*sizeof(char *));
	memcpy(data_string, shuffled_strings, n*sizeof(char *));
	testQuicksortString(data_string, n);
	memcpy(data_string, shuffled_strings, n*sizeof(char *));
	testNativeSortString(data_string, n);


	/* object sorting */
	printf("objects:\n");

	/* make student objects based on the sorted strings */
	clock_gettime(CLOCK_REALTIME, &start_time);
	sorted_students = (student **) malloc(n*sizeof(student *));
	for (i = 0; i < n; ++i) {
		sorted_students[i] = new_student(sorted_strings[i], i%10);
	}
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    made student objects");

	/* permute randomly */
	shuffled_students = (student **) malloc(n*sizeof(student *));
	memcpy(shuffled_students, sorted_students, n*sizeof(student *));
	clock_gettime(CLOCK_REALTIME, &start_time);
	shuffle((void **)shuffled_students, n);
	clock_gettime(CLOCK_REALTIME, &stop_time);
	displayElapsed("    shuffled");

	/* sort objects */

	data_student = (student **) malloc(n*sizeof(student *));
	memcpy(data_student, shuffled_students, n*sizeof(student *));
	testQuicksortStudent(data_student, n);
	memcpy(data_student, shuffled_students, n*sizeof(student *));
	testNativeSortStudent(data_student, n);

	return 0;
}
