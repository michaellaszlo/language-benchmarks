import java.lang.*;
import java.util.*;
import java.io.*;

class Student implements Comparable<Student> {
	String name;
	int grade;
	Student(String name, int grade) {
		this.name = name;
		this.grade = grade;
	}
	public String toString() {
		return grade+" "+name;
	}
	public int compareTo(Student other) {
		if (this.grade != other.grade) {
			return this.grade - other.grade;
		}
		return this.name.compareTo(other.name);
	}
}

public class Sorting {

	//--- heapsort: in-place implementation with linear-time heap construction

	static <T extends Comparable<T>> void trickleDown(ArrayList<T> data,
			int pos, int n) {
		int left = 2*pos+1;
		if (left >= n) {
			return;
		}
		int right = 2*pos+2;
		if (right >= n) {
			if (data.get(pos).compareTo(data.get(left)) < 0) {
				Collections.swap(data, pos, left);
			}
			return;
		}
		if (data.get(pos).compareTo(data.get(left)) < 0) {
			if (data.get(left).compareTo(data.get(right)) < 0) {
				Collections.swap(data, pos, right);
				trickleDown(data, right, n);
			} else {
				Collections.swap(data, pos, left);
				trickleDown(data, left, n);
			}
		} else if(data.get(pos).compareTo(data.get(right)) < 0) {
			Collections.swap(data, pos, right);
			trickleDown(data, right, n);
		}
	}

	static <T extends Comparable<T>> void heapsort(ArrayList<T> data) {
		// build a max-heap
		int n = data.size();
		for (int i = (n-2)/2; i != -1; --i) {
			trickleDown(data, i, n);
		}

		// successively remove the max item and swap it to the end
		for (int i = n-1; i != -1; --i) {
			Collections.swap(data, i, 0);
			trickleDown(data, 0, i);
		}
	}

	static <T extends Comparable<T>> void testHeapsort(ArrayList<T> data) {
		double startTime = System.currentTimeMillis();
		heapsort(data);
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran heapsort  %.3f", elapsed));
		if (!isSorted(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- mergesort: recursive implementation with two arrays

	static <T extends Comparable<T>> void mergesort(ArrayList<T> data, ArrayList<T> buffer,
			int left, int right) {
		if (left+1 == right) {
			return;
		}
		int mid = (left+right)/2;
		mergesort(data, buffer, left, mid);
		mergesort(data, buffer, mid, right);
		int leftPos = left, rightPos = mid,
			bufferPos = left;
		while (true) {
			if (data.get(leftPos).compareTo(data.get(rightPos)) <= 0) {
				buffer.set(bufferPos++, data.get(leftPos++));
				if (leftPos == mid) {
					while (rightPos != right) {
						buffer.set(bufferPos++, data.get(rightPos++));
					}
					break;
				}
			} else {
				buffer.set(bufferPos++, data.get(rightPos++));
				if (rightPos == right) {
					while (leftPos != mid) {
						buffer.set(bufferPos++, data.get(leftPos++));
					}
					break;
				}
			}
		}
		for (int i = left; i != right; ++i) {
			data.set(i, buffer.get(i));
		}
	}

	static <T extends Comparable<T>> void testMergesort(ArrayList<T> data) {
		double startTime = System.currentTimeMillis();
		ArrayList<T> buffer = new ArrayList<T>(data);
		mergesort(data, buffer, 0, data.size());
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran mergesort  %.3f", elapsed));
		if (!isSorted(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- quicksort: in-place implementation with median-of-three pivot

	static <T extends Comparable<T>> void quicksort(ArrayList<T> data, int left, int right) {
		if (left+1 >= right) {
			return;
		}
		int ai = left, bi = (left+right)/2, ci = right-1, pos;
		T a = data.get(ai), b = data.get(bi), c = data.get(ci);
		if (a.compareTo(b) < 0) {
			if (c.compareTo(a) < 0) {
				pos = ai;
			} else if (c.compareTo(b) < 0) {
				pos = ci;
			} else {
				pos = bi;
			}
		} else {
			if (c.compareTo(b) < 0) {
				pos = bi;
			} else if (c.compareTo(a) < 0) {
				pos = ci;
			} else {
				pos = ai;
			}
		}
		T pivot = data.get(pos);
		Collections.swap(data, right-1, pos);
		int tail = left;
		for (int i = left; i != right-1; ++i) {
			if (data.get(i).compareTo(pivot) < 0) {
				Collections.swap(data, tail, i);
				++tail;
			}
		}
		Collections.swap(data, right-1, tail);
		quicksort(data, left, tail);
		quicksort(data, tail+1, right);
	}

	static <T extends Comparable<T>> void testQuicksort(ArrayList<T> data) {
		double startTime = System.currentTimeMillis();
		quicksort(data, 0, data.size());
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran quicksort  %.3f", elapsed));
		if (!isSorted(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- native sort

	static <T extends Comparable<T>> void testNativeSort(ArrayList<T> data) {
		double startTime = System.currentTimeMillis();
		Collections.sort(data);
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran native sort  %.3f", elapsed));
		if (!isSorted(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- random permutation

	static <T extends Comparable<T>> void shuffle(ArrayList<T> data) {
		// constants for a linear congruential generator
		long m = (1L << 32),
			 a = 1664525,
			 c = 1013904223,
			 randomValue = 42;  // seed
		for (int i = data.size()-1; i != 0; --i) {
			randomValue = (a*randomValue + c) % m;
			int j = (int)(randomValue % (long)(i+1));
			if (j != i) {
				Collections.swap(data, i, j);
			}
		}
	}


	//--- helper methods

	static <T extends Comparable<T>> boolean isSorted(ArrayList<T> data) {
		int n = data.size();
		for (int i = 1; i != n; ++i) {
			if (data.get(i-1).compareTo(data.get(i)) > 0) {
				return false;
			}
		}
		return true;
	}

	static <T extends Comparable<T>> void display(ArrayList<T> data) {
		System.out.print(data.get(0));
		for (int i = 1; i != data.size(); ++i) {
			System.out.print(" "+data.get(i));
		}
		System.out.println();
	}


	public static void main(String args[]) {
		if (args.length != 1) {
			System.out.println("error: must specify the length of the array");
			return;
		}
		int n = Integer.valueOf(args[0]);

		//--- integer sorting
		System.out.println("integers:");

		// initialize an array with 0, ..., n-1
		double startTime = System.currentTimeMillis();
		ArrayList<Integer> sortedIntegers = new ArrayList<Integer>();
		for (int i = 0; i < n; ++i) {
			sortedIntegers.add(i);
		}
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
				"    made ordered integer array  %.3f", elapsed));

		// permute randomly
		ArrayList<Integer> shuffledIntegers =
				new ArrayList<Integer>(sortedIntegers);
		startTime = System.currentTimeMillis();
		shuffle(shuffledIntegers);
		elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format("    shuffled  %.3f", elapsed));

		// sort integer arrays
		testHeapsort(new ArrayList<Integer>(shuffledIntegers));
		testMergesort(new ArrayList<Integer>(shuffledIntegers));
		testQuicksort(new ArrayList<Integer>(shuffledIntegers));
		testNativeSort(new ArrayList<Integer>(shuffledIntegers));


		//--- string sorting
		System.out.println("strings:");

		// make strings: 'a', 'b', ..., 'z', 'aa', 'ab', ..., 'az', 'ba', ...
		startTime = System.currentTimeMillis();

		ArrayList<String> sortedStrings = new ArrayList<String>();
		StringBuffer buf = new StringBuffer("a");
		int numChars = 1;
		for (int i = 0; i != n; ++i) {
			sortedStrings.add(buf.toString());
			int pos = numChars-1;
			while (true) {
				if (buf.charAt(pos) != 'z') {
					buf.setCharAt(pos, (char)(buf.charAt(pos)+1));
					break;
				}
				pos -= 1;
				if (pos == -1) {
					buf.insert(0, 'a');
					++numChars;
					break;
				}
			}
			if (pos != numChars-1) {
				for (int j = pos+1; j <= numChars-1; ++j) {
					buf.setCharAt(j, 'a');
				}
			}
		}
		elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
				"    made ordered string array  %.3f", elapsed));

		// permute randomly
		ArrayList<String> shuffledStrings =
				new ArrayList<String>(sortedStrings);
		startTime = System.currentTimeMillis();
		shuffle(shuffledStrings);
		elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format("    shuffled  %.3f", elapsed));

		// sort string arrays
		testQuicksort(new ArrayList<String>(shuffledStrings));
		testNativeSort(new ArrayList<String>(shuffledStrings));


		//--- object sorting
		System.out.println("objects:");

		// make student objects based on the sorted strings
		startTime = System.currentTimeMillis();
		ArrayList<Student> sortedStudents = new ArrayList<Student>();
		for (int i = 0; i < n; ++i) {
			sortedStudents.add(new Student(sortedStrings.get(i), i%10));
		}
		elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
				"    made student objects  %.3f", elapsed));

		// permute randomly
		ArrayList<Student> shuffledStudents =
				new ArrayList<Student>(sortedStudents);
		startTime = System.currentTimeMillis();
		shuffle(shuffledStudents);
		elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format("    shuffled  %.3f", elapsed));

		// sort objects
		testQuicksort(new ArrayList<Student>(shuffledStudents));
		testNativeSort(new ArrayList<Student>(shuffledStudents));
	}
}
