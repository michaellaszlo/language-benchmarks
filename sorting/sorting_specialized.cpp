#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;


template<typename T> bool isSorted(vector<T> &data);
template<typename T> void display(vector<T> &data);


//--- heapsort: in-place implementation with linear-time heap construction

void trickle_down(vector<int> &data, int pos, int n) {
	int left = 2*pos+1;
	if (left >= n) {
		return;
	}
	int right = 2*pos+2;
	if (right >= n) {
		if (data[pos] < data[left]) {
			swap(data[pos], data[left]);
		}
		return;
	}
	if (data[pos] < data[left]) {
		if (data[left] < data[right]) {
			swap(data[pos], data[right]);
			trickle_down(data, right, n);
		} else {
			swap(data[pos], data[left]);
			trickle_down(data, left, n);
		}
	} else if(data[pos] < data[right]) {
		swap(data[pos], data[right]);
		trickle_down(data, right, n);
	}
}

void heapsort(vector<int> &data, int n) {
	// build a max-heap
	for (int i = (n-2)/2; i != -1; --i) {
		trickle_down(data, i, n);
	}

	// successively remove the max item and swap it to the end
	for (int i = n-1; i != -1; --i) {
		swap(data[i], data[0]);
		trickle_down(data, 0, i);
	}
}

void testHeapsort(vector<int> data) {
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();
	heapsort(data, data.size());
	end = chrono::system_clock::now();
	double elapsed = chrono::duration<double>(end-start).count();
	printf("    ran heapsort in %.3f s\n", elapsed);
	if (!isSorted(data)) {
		cout << "error: data is not sorted\n";
	}
}

void trickle_down(vector<string> &data, int pos, int n) {
	int left = 2*pos+1;
	if (left >= n) {
		return;
	}
	int right = 2*pos+2;
	if (right >= n) {
		if (data[pos] < data[left]) {
			swap(data[pos], data[left]);
		}
		return;
	}
	if (data[pos] < data[left]) {
		if (data[left] < data[right]) {
			swap(data[pos], data[right]);
			trickle_down(data, right, n);
		} else {
			swap(data[pos], data[left]);
			trickle_down(data, left, n);
		}
	} else if(data[pos] < data[right]) {
		swap(data[pos], data[right]);
		trickle_down(data, right, n);
	}
}

void heapsort(vector<string> &data, int n) {
	// build a max-heap
	for (int i = (n-2)/2; i != -1; --i) {
		trickle_down(data, i, n);
	}

	// successively remove the max item and swap it to the end
	for (int i = n-1; i != -1; --i) {
		swap(data[i], data[0]);
		trickle_down(data, 0, i);
	}
}

void testHeapsort(vector<string> data) {
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();
	heapsort(data, data.size());
	end = chrono::system_clock::now();
	double elapsed = chrono::duration<double>(end-start).count();
	printf("    ran heapsort in %.3f s\n", elapsed);
	if (!isSorted(data)) {
		cout << "error: data is not sorted\n";
	}
}


//--- mergesort: recursive implementation with two arrays

void mergesort(vector<int> &data, vector<int> &buffer, int left, int right) {
	if (left+1 == right) {
		return;
	}
	int mid = (left+right)/2;
	mergesort(data, buffer, left, mid);
	mergesort(data, buffer, mid, right);
	int posLeft = left, posRight = mid,
		posBuffer = left;
	while (true) {
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
	for (int i = left; i != right; ++i) {
		data[i] = buffer[i];
	}
}

void testMergesort(vector<int> data) {
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();
	vector<int> buffer = vector<int>(data.size());
	mergesort(data, buffer, 0, data.size());
	end = chrono::system_clock::now();
	double elapsed = chrono::duration<double>(end-start).count();
	printf("    ran mergesort in %.3f s\n", elapsed);
	if (!isSorted(data)) {
		cout << "error: data is not sorted\n";
	}
}

void mergesort(vector<string> &data, vector<string> &buffer,
		int left, int right) {
	if (left+1 == right) {
		return;
	}
	int mid = (left+right)/2;
	mergesort(data, buffer, left, mid);
	mergesort(data, buffer, mid, right);
	int posLeft = left, posRight = mid,
		posBuffer = left;
	while (true) {
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
	for (int i = left; i != right; ++i) {
		data[i] = buffer[i];
	}
}

void testMergesort(vector<string> data) {
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();
	vector<string> buffer = vector<string>(data.size());
	mergesort(data, buffer, 0, data.size());
	end = chrono::system_clock::now();
	double elapsed = chrono::duration<double>(end-start).count();
	printf("    ran mergesort in %.3f s\n", elapsed);
	if (!isSorted(data)) {
		cout << "error: data is not sorted\n";
	}
}


//--- quicksort: in-place implementation with median-of-three pivot selection

void quicksort(vector<int> &data, int left, int right) {
	if (left+1 >= right) {
		return;
	}
	int ai = left, bi = (left+right)/2, ci = right-1, pos,
		a = data[ai], b = data[bi], c = data[ci];
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
    int pivot = data[pos];
	data[pos] = data[right-1];
	int tail = left;
	for (int i = left; i != right-1; ++i) {
		if (data[i] < pivot) {
			swap(data[tail], data[i]);
			++tail;
		}
	}
	data[right-1] = data[tail];
	data[tail] = pivot;
    quicksort(data, left, tail);
    quicksort(data, tail+1, right);
}

void testQuicksort(vector<int> data) {
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();
	quicksort(data, 0, data.size());
	end = chrono::system_clock::now();
	double elapsed = chrono::duration<double>(end-start).count();
	printf("    ran quicksort in %.3f s\n", elapsed);
	if (!isSorted(data)) {
		cout << "error: data is not sorted\n";
	}
}

void quicksort(vector<string> &data, int left, int right) {
	if (left+1 >= right) {
		return;
	}
	int ai = left, bi = (left+right)/2, ci = right-1, pos;
	string a = data[ai], b = data[bi], c = data[ci];
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
    string pivot = data[pos];
	data[pos] = data[right-1];
	int tail = left;
	for (int i = left; i != right-1; ++i) {
		if (data[i] < pivot) {
			swap(data[tail], data[i]);
			++tail;
		}
	}
	data[right-1] = data[tail];
	data[tail] = pivot;
    quicksort(data, left, tail);
    quicksort(data, tail+1, right);
}

void testQuicksort(vector<string> data) {
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();
	quicksort(data, 0, data.size());
	end = chrono::system_clock::now();
	double elapsed = chrono::duration<double>(end-start).count();
	printf("    ran quicksort in %.3f s\n", elapsed);
	if (!isSorted(data)) {
		cout << "error: data is not sorted\n";
	}
}


//--- native sorting on integers or strings

void testNativeSort(vector<int> data) {
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();
	sort(data.begin(), data.end());
	end = chrono::system_clock::now();
	double elapsed = chrono::duration<double>(end-start).count();
	printf("    ran native integer sort in %.3f s\n", elapsed);
	if (!isSorted(data)) {
		cout << "error: data is not sorted\n";
	}
}

void testNativeSort(vector<string> data) {
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();
	sort(data.begin(), data.end());
	end = chrono::system_clock::now();
	double elapsed = chrono::duration<double>(end-start).count();
	printf("    ran native string sort in %.3f s\n", elapsed);
	if (!isSorted(data)) {
		cout << "error: data is not sorted\n";
	}
}

void permute(vector<int> &data) {
	// constants for a linear congruential generator
	long long m = (1L << 32),
		 a = 1664525,
		 c = 1013904223,
		 randomValue = 42;  // seed
    for (int i = data.size()-1; i != 0; --i) {
        randomValue = (a*randomValue + c) % m;
        int j = randomValue % (i+1);
        if (j != i) {
			swap(data[i], data[j]);
        }
    }
}

void permute(vector<string> &data) {
	// constants for a linear congruential generator
	long long m = (1L << 32),
		 a = 1664525,
		 c = 1013904223,
		 randomValue = 42;  // seed
    for (int i = data.size()-1; i != 0; --i) {
        randomValue = (a*randomValue + c) % m;
        int j = randomValue % (i+1);
        if (j != i) {
			swap(data[i], data[j]);
        }
    }
}


template<typename T> void display(vector<T> &data) {
	typename vector<T>::iterator it = data.begin();
    cout << *it;
    for (++it; it != data.end(); ++it) {
        cout << " " << *it;
    }
    cout << '\n';
}

template<typename T> bool isSorted(vector<T> &data) {
	int n = data.size();
	for (int i = 1; i < n; ++i) {
		if (data[i-1] > data[i]) {
			return false;
		}
	}
	return true;
}


int main(int argc, char **argv) {
	if (argc != 2) {
		cout << "error: must specify the length of the array\n";
		return 0;
	}
	int n = atoi(argv[1]);


	//--- integer sorting
	cout << "integers:\n";

	// initialize an array with 0, ..., n-1
	chrono::time_point<chrono::system_clock> start, end;
	start = chrono::system_clock::now();
	vector<int> unsorted_integers(n);
	for (int i = 0; i < n; ++i) {
		unsorted_integers[i] = i;
	}
	end = chrono::system_clock::now();
	double elapsed = chrono::duration<double>(end-start).count();
	printf("    made ordered integer vector in %.3f s\n", elapsed);

	start = chrono::system_clock::now();
	permute(unsorted_integers);
	end = chrono::system_clock::now();
	elapsed = chrono::duration<double>(end-start).count();
	printf("    permuted randomly in %.3f s\n", elapsed);

	// sort integer vectors
	testHeapsort(unsorted_integers);
	testMergesort(unsorted_integers);
	testQuicksort(unsorted_integers);
	testNativeSort(unsorted_integers);


	//--- string sorting
	cout << "strings:\n";

	start = chrono::system_clock::now();
    vector<string> unsorted_strings(n);
	string s("a");
    int num_chars = 1;
	for (int i = 0; i != n; ++i) {
		unsorted_strings[i] = s;
		int pos = num_chars-1;
		while (true) {
			if (s[pos] != 'z') {
				++s[pos];
				break;
			}
			pos -= 1;
			if (pos == -1) {
				s.insert(s.begin(), 'a');
				num_chars += 1;
				break;
			}
		}
		if (pos != num_chars-1) {
			for (int j = pos+1; j <= num_chars-1; ++j) {
				s[j] = 'a';
			}
		}
	}
	end = chrono::system_clock::now();
	elapsed = chrono::duration<double>(end-start).count();
	printf("    made ordered string vector in %.3f s\n", elapsed);

    // permute it randomly
	start = chrono::system_clock::now();
	permute(unsorted_strings);
	end = chrono::system_clock::now();
	elapsed = chrono::duration<double>(end-start).count();
	printf("    permuted randomly in %.3f s\n", elapsed);

    // sort string arrays
    testNativeSort(unsorted_strings);

    return 0;
}
