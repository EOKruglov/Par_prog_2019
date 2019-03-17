#include "RadixSort.h"

int main()
{
	int *arr;
	int *tmp;
	int size = 10000;

	arr = new int[size];
	tmp = new int[size];

	ArrayFill(arr, size);

	for (int i = 0; i < size; i++)
	{
		tmp[i] = arr[i];
	}

	RadixSort(arr, size);

	if (CheckResult(arr, tmp, size))
	{
		cout << "Correct result" << endl;
	}

	else
	{
		cout << "Incorrect result" << endl;
	}

	return 0;
}