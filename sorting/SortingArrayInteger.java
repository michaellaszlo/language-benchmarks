import java.lang.*;
import java.util.*;
import java.io.*;

public class SortingArrayInteger {


	//--- heapsort: in-place implementation with linear-time heap construction

	static void trickleDown(Integer data[], int pos, int n) {
		int left = 2*pos+1;
		if (left >= n) {
			return;
		}
		int right = 2*pos+2;
		if (right >= n) {
			if (data[pos] < data[left]) {
				Integer t = data[pos];
				data[pos] = data[left];
				data[left] = t;
			}
			return;
		}
		if (data[pos] < data[left]) {
			if (data[left] < data[right]) {
				Integer t = data[pos];
				data[pos] = data[right];
				data[right] = t;
				trickleDown(data, right, n);
			} else {
				Integer t = data[pos];
				data[pos] = data[left];
				data[left] = t;
				trickleDown(data, left, n);
			}
		} else if(data[pos] < data[right]) {
			Integer t = data[pos];
			data[pos] = data[right];
			data[right] = t;
			trickleDown(data, right, n);
		}
	}

	static void heapsort(Integer data[]) {
		// build a max-heap
		int n = data.length;
		for (int i = (n-2)/2; i != -1; --i) {
			trickleDown(data, i, n);
		}

		// successively remove the max item and swap it to the end
		for (int i = n-1; i != -1; --i) {
			Integer t = data[i];
			data[i] = data[0];
			data[0] = t;
			trickleDown(data, 0, i);
		}
	}

	static void testHeapsort(Integer data[]) {
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

	static void mergesort(Integer data[], Integer buffer[],
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
		for (int i = left; i != right; ++i) {
			data[i] = buffer[i];
		}
	}

	static void testMergesort(Integer data[]) {
		double startTime = System.currentTimeMillis();
		Integer buffer[] = new Integer[data.length];
		mergesort(data, buffer, 0, data.length);
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran mergesort in %.3f", elapsed));
		if (!isSortedInteger(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- quicksort: in-place implementation with median-of-three pivot

	static void quicksort(Integer data[], int left, int right) {
		if (left+1 >= right) {
			return;
		}
		int ai = left, bi = (left+right)/2, ci = right-1, pos;
		Integer a = data[ai], b = data[bi], c = data[ci];
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
		Integer pivot = data[pos];
		data[pos] = data[right-1];
		int tail = left;
		for (int i = left; i != right-1; ++i) {
			if (data[i] < pivot) {
				Integer t = data[tail];
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

	static void testQuicksort(Integer data[]) {
		double startTime = System.currentTimeMillis();
		quicksort(data, 0, data.length);
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran quicksort in %.3f", elapsed));
		if (!isSortedInteger(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- native sort on integers

	static void testNativeSortInteger(Integer data[]) {
		double startTime = System.currentTimeMillis();
		Arrays.sort(data);
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran native integer sort in %.3f", elapsed));
		if (!isSortedInteger(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- native sort on strings

	static void testNativeSortString(String data[]) {
		double startTime = System.currentTimeMillis();
		Arrays.sort(data);
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    ran native string sort in %.3f", elapsed));
		if (!isSortedString(data)) {
			System.out.println("error: data is not sorted");
		}
	}


	//--- random permutation

	static void permuteInts(Integer data[]) {
		// constants for a linear congruential generator
		long m = (1L << 32),
			 a = 1664525,
			 c = 1013904223,
			 randomValue = 42;  // seed
		for (int i = data.length-1; i != 0; --i) {
			randomValue = (a*randomValue + c) % m;
			int j = (int)(randomValue % (long)(i+1));
			if (j != i) {
				Integer t = data[i];
				data[i] = data[j];
				data[j] = t;
			}
		}
	}

	static void permuteStrings(String data[]) {
		// constants for a linear congruential generator
		long m = (1L << 32),
			 a = 1664525,
			 c = 1013904223,
			 randomValue = 42;  // seed
		for (int i = data.length-1; i != 0; --i) {
			randomValue = (a*randomValue + c) % m;
			int j = (int)(randomValue % (long)(i+1));
			if (j != i) {
				String t = data[i];
				data[i] = data[j];
				data[j] = t;
			}
		}
	}


	static boolean isSortedInteger(Integer data[]) {
		int n = data.length;
		for (int i = 1; i != n; ++i) {
			if (data[i-1] > data[i]) {
				return false;
			}
		}
		return true;
	}

	static boolean isSortedString(String data[]) {
		int n = data.length;
		for (int i = 1; i != n; ++i) {
			if (data[i-1].compareTo(data[i]) > 0) {
				return false;
			}
		}
		return true;
	}

	static void displayInts(Integer data[]) {
		System.out.print(data[0]);
		for (int i = 1; i != data.length; ++i) {
			System.out.print(" "+data[i]);
		}
		System.out.println();
	}

	static void displayStrings(String data[]) {
		System.out.print(data[0]);
		for (int i = 1; i != data.length; ++i) {
			System.out.print(" "+data[i]);
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
		Integer unsortedIntegers[] = new Integer[n];
		for (int i = 0; i < n; ++i) {
			unsortedIntegers[i] = i;
		}
		double elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    made ordered integer array in %.3f", elapsed));

		// permute it randomly
		startTime = System.currentTimeMillis();
		permuteInts(unsortedIntegers);
		elapsed = (System.currentTimeMillis() - startTime)/1000;
		System.out.println(String.format(
					"    permuted randomly in %.3f", elapsed));

		// sort integer arrays
		testHeapsort(Arrays.copyOf(unsortedIntegers, n));
		testMergesort(Arrays.copyOf(unsortedIntegers, n));
		testQuicksort(Arrays.copyOf(unsortedIntegers, n));
		testNativeSortInteger(Arrays.copyOf(unsortedIntegers, n));
	}
}
