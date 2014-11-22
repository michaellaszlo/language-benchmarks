import java.lang.*;
import java.util.*;
import java.io.*;

public class SortingArrayListInteger {


	//--- heapsort: in-place implementation with linear-time heap construction

	static void trickleDown(ArrayList<Integer> data, int pos, int n) {
		int left = 2*pos+1;
		if (left >= n) {
			return;
		}
		int right = 2*pos+2;
		if (right >= n) {
			if (data.get(pos) < data.get(left)) {
				Collections.swap(data, pos, left);
			}
			return;
		}
		if (data.get(pos) < data.get(left)) {
			if (data.get(left) < data.get(right)) {
				Collections.swap(data, pos, right);
				trickleDown(data, right, n);
			} else {
				Collections.swap(data, pos, left);
				trickleDown(data, left, n);
			}
		} else if(data.get(pos) < data.get(right)) {
			Collections.swap(data, pos, right);
			trickleDown(data, right, n);
		}
	}

	static void heapsort(ArrayList<Integer> data) {
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

	static void testHeapsort(ArrayList<Integer> data) {
		double startTime = System.currentTimeMillis();
		heapsort(data);
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran heapsort in %.3f", elapsed));
		if (!isSortedInteger(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- mergesort: recursive implementation with two arrays

	static void mergesort(ArrayList<Integer> data, ArrayList<Integer> buffer,
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
			if (data.get(leftPos) <= data.get(rightPos)) {
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

	static void testMergesort(ArrayList<Integer> data) {
		double startTime = System.currentTimeMillis();
		ArrayList<Integer> buffer = new ArrayList<Integer>(data);
		mergesort(data, buffer, 0, data.size());
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran mergesort in %.3f", elapsed));
		if (!isSortedInteger(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- quicksort: in-place implementation with median-of-three pivot

	static void quicksort(ArrayList<Integer> data, int left, int right) {
		if (left+1 >= right) {
			return;
		}
		int ai = left, bi = (left+right)/2, ci = right-1, pos;
		Integer a = data.get(ai), b = data.get(bi), c = data.get(ci);
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
		Integer pivot = data.get(pos);
		Collections.swap(data, right-1, pos);
		int tail = left;
		for (int i = left; i != right-1; ++i) {
			if (data.get(i) < pivot) {
				Collections.swap(data, tail, i);
				++tail;
			}
		}
		Collections.swap(data, right-1, tail);
		quicksort(data, left, tail);
		quicksort(data, tail+1, right);
	}

	static void testQuicksort(ArrayList<Integer> data) {
		double startTime = System.currentTimeMillis();
		quicksort(data, 0, data.size());
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran quicksort in %.3f", elapsed));
		if (!isSortedInteger(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- native sort on integers

	static void testNativeSortInteger(ArrayList<Integer> data) {
		double startTime = System.currentTimeMillis();
		Collections.sort(data);
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran native integer sort in %.3f", elapsed));
		if (!isSortedInteger(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- native sort on strings

	static void testNativeSortString(ArrayList<String> data) {
		double startTime = System.currentTimeMillis();
		Collections.sort(data);
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran native string sort in %.3f", elapsed));
		if (!isSortedString(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- random permutation

	static void permuteIntegers(ArrayList<Integer> data) {
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

	static void permuteStrings(ArrayList<String> data) {
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


	static boolean isSortedInteger(ArrayList<Integer> data) {
		int n = data.size();
		for (int i = 1; i != n; ++i) {
			if (data.get(i-1) > data.get(i)) {
				return false;
			}
		}
		return true;
	}

	static boolean isSortedString(ArrayList<String> data) {
		int n = data.size();
		for (int i = 1; i != n; ++i) {
			if (data.get(i-1).compareTo(data.get(i)) > 0) {
				return false;
			}
		}
		return true;
	}

	static void displayIntegers(ArrayList<Integer> data) {
		System.out.print(data.get(0));
		for (int i = 1; i != data.size(); ++i) {
			System.out.print(" "+data.get(i));
		}
		System.out.println();
	}

	static void displayStrings(ArrayList<String> data) {
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
		ArrayList<Integer> unsortedIntegers = new ArrayList<Integer>();
		for (int i = 0; i < n; ++i) {
			unsortedIntegers.add(i);
		}
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    made ordered integer array in %.3f", elapsed));

		// permute it randomly
		startTime = System.currentTimeMillis();
		permuteIntegers(unsortedIntegers);
		elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    permuted randomly in %.3f", elapsed));

		// sort integer arrays
		testHeapsort(new ArrayList<Integer>(unsortedIntegers));
		testMergesort(new ArrayList<Integer>(unsortedIntegers));
		testQuicksort(new ArrayList<Integer>(unsortedIntegers));
		testNativeSortInteger(new ArrayList<Integer>(unsortedIntegers));
	}
}
