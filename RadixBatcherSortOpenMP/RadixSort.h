#pragma once

#include <iostream>
#include <time.h>
#include <algorithm>
#include <vector>

using namespace std;

void Radix(int byte, int size, vector<int> sourceArr, vector<int> destArr)
{
	vector<int> count(256, 0);
	vector<int> offset(256, 0);

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

void RadixSort(vector<int> sourceArr, int size)
{
	vector <int> temp(size);
	Radix(0, size, sourceArr, temp);
	Radix(1, size, temp, sourceArr);
	Radix(2, size, sourceArr, temp);
	Radix(3, size, temp, sourceArr);
}

void ArrayFill(vector<int> arr, int size)
{
	//srand(time(NULL));
	srand(6);
	for (int i = 0; i < size; i++)
		arr.push_back(rand());
}

bool CheckResult(vector<int> arr, vector<int> tmp, int size)
{
	sort(tmp.begin(), tmp.end());

	if (tmp != arr)
	{
		return false;
	}

	else
	{
		return true;
	}
}