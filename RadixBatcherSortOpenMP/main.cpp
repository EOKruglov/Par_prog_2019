#include "RadixSort.h"

int main()
{
	int size = 10000;

	vector<int> arr(size);
	vector<int> tmp(size);

	ArrayFill(arr, size);

	tmp = arr;

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