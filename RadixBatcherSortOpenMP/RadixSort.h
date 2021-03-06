#pragma once

#include <iostream>
#include <time.h>
#include <algorithm>
#include <vector>


void ArrayFill(std::vector<int> &arr, int size)
{
	//srand(time(NULL));
	srand(6);
	for (int i = 0; i < size; i++)
		arr[i] = rand() % 100 + 1;
}


bool CheckResult(std::vector<int> &arr, std::vector<int> &tmp)
{
	if (tmp != arr)
	{
		return false;
	}

	else
	{
		return true;
	}
}


class SequentialSort
{
public:

	void Radix(int byte, int size, std::vector<int> &sourceArr, std::vector<int> &destArr)
	{
		std::vector<int> count(256, 0);
		std::vector<int> offset(256, 0);

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

	void RadixSort(std::vector<int> &sourceArr, int size)
	{
		std::vector <int> temp(size);
		Radix(0, size, sourceArr, temp);
		Radix(1, size, temp, sourceArr);
		Radix(2, size, sourceArr, temp);
		Radix(3, size, temp, sourceArr);
	}


};


class OmpSort :SequentialSort
{
public:

	void RadixSort(std::vector<int> &sourceArr, int start, int finish)
	{
		int _size = (finish - start) + 1;
		std::vector<int> temp(_size);
		std::vector<int> buffer_array;

		for (int i = 0; i < _size; i++)
		{
			buffer_array.push_back(sourceArr[start + i]);
		}

		Radix(0, _size, buffer_array, temp);
		Radix(1, _size, temp, buffer_array);
		Radix(2, _size, buffer_array, temp);
		Radix(3, _size, temp, buffer_array);

		for (int i = 0; i < _size; i++)
		{
			sourceArr[start + i] = buffer_array[i];
		}
	}



	void EvenSplit(std::vector<int> &arr, int start1, int finish1, int start2, int finish2)
	{
		int size1 = (finish1 - start1) + 1;
		int size2 = (finish2 - start2) + 1;

		std::vector<int> tmp;
		std::vector<int> arr2;

		for (int i = 0; i < size1; i++)
		{
			tmp.push_back(arr[i + start1]);
		}

		for (int i = 0; i < size2; i++)
		{
			arr2.push_back(arr[i + start2]);
		}


		int a = 0;
		int b = 0;
		int i = 0;

		while ((a < size1) && (b < size2))
		{
			if (tmp[a] < arr2[b])
			{
				arr[i + start1] = tmp[a];
				a += 2;
			}

			else
			{
				arr[i + start1] = arr2[b];
				b += 2;
			}

			i += 2;
		}

		if (a == size1)
			for (int j = b; j < size2; j += 2, i += 2)
				arr[i + start1] = arr2[j];
		else
			for (int j = a; j < size1; j += 2, i += 2)
				arr[i + start1] = tmp[j];
	}


	void OddSplit(std::vector<int> &arr, int start1, int finish1, int start2, int finish2)
	{

		int size1 = (finish1 - start1) + 1;
		int size2 = (finish2 - start2) + 1;

		std::vector<int> tmp;
		std::vector<int> arr2;

		for (int i = 0; i < size1; i++)
		{
			tmp.push_back(arr[i + start1]);
		}

		for (int i = 0; i < size2; i++)
		{
			arr2.push_back(arr[i + start2]);
		}

		int a = 1;
		int b = 1;
		int i = 1;

		while ((a < size1) && (b < size2))
		{
			if (tmp[a] < arr2[b])
			{
				arr[i + start1] = tmp[a];
				a += 2;
			}

			else
			{
				arr[i + start1] = arr2[b];
				b += 2;
			}

			i += 2;
		}

		if (a == size1)
			for (int j = b; j < size2; j += 2, i += 2)
				arr[i + start1] = arr2[j];
		else
			for (int j = a; j < size1; j += 2, i += 2)
				arr[i + start1] = tmp[j];

	}


	void Comparator(std::vector<int> &arr, int start, int finish)
	{
		int size = (finish - start) + 1;
		for (int i = 1; i < size; i++)
			if (arr[i + start] < arr[i + start - 1])
				std::swap(arr[i + start], arr[i + start - 1]);
	}

	void BatcherMerge(std::vector<int> &arr, int start1, int finish1, int start2, int finish2)
	{
		//int size1 = (finish1 - start1) + 1;
		//int size2 = (finish2 - start2) + 1;
		//vector<int> res = arr;
		

		EvenSplit(arr, start1, finish1, start2, finish2);
		OddSplit(arr, start1, finish1, start2, finish2);
		Comparator(arr, start1, finish2);
		//arr = res;
	}


};
