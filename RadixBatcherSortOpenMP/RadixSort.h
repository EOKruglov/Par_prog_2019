#pragma once

#include <iostream>
#include <time.h>
#include <algorithm>

using namespace std;

void Radix(int byte, int size, int *sourceArr, int *destArr)
{
	int count[256];
	int offset[256];
	memset(count, 0, sizeof(count));

	for (int i = 0; i < size; i++)
	{
		if (byte == 3)
			count[((sourceArr[i] >> (byte * 8)) + 128) & 0xff]++;
		else
			count[((sourceArr[i]) >> (byte * 8)) & 0xff]++;
	}

	offset[0] = 0;
	for (int i = 1; i < 256; ++i)
		offset[i] = offset[i - 1] + count[i - 1];


	for (int i = 0; i < size; ++i)
	{
		if (byte == 3)
			destArr[offset[((sourceArr[i] >> (byte * 8)) + 128) & 0xff]++] = sourceArr[i];
		else
			destArr[offset[((sourceArr[i]) >> (byte * 8)) & 0xff]++] = sourceArr[i];
	}
}

void RadixSort(int *sourceArr, int size)
{
	int *temp = new int[size];
	Radix(0, size, sourceArr, temp);
	Radix(1, size, temp, sourceArr);
	Radix(2, size, sourceArr, temp);
	Radix(3, size, temp, sourceArr);
	delete[] temp;
}

void ArrayFill(int *arr, int size)
{
	//srand(time(NULL));
	srand(6);
	for (int i = 0; i < size; i++)
		arr[i] = rand();
}

bool CheckResult(int *arr, int *tmp, int size)
{
	sort(tmp, tmp + size);

	for (int i = 0; i < size; i++)
	{
		if (tmp[i] != arr[i])
		{
			return false;
		}
	}

	return true;
}