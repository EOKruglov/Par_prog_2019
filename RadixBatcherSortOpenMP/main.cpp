#include "RadixSort.h"

int main()
{
	int size = 1000000;

	vector<int> arr(size);
	vector<int> tmp(size);
	SequentialSort ss;


	ArrayFill(arr, size);

	tmp = arr;

	ss.RadixSort(arr, size);

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