#include "RadixSort.h"

int main()
{
	int *arr;
	int size = 10000;

	arr = new int[size];

	ArrayFill(arr, size);
	RadixSort(arr, size);

	return 0;
}