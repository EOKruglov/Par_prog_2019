#pragma once

#include <vector>

#include "tbb/task_scheduler_init.h"
#include "tbb/task_group.h"
#include "tbb/tick_count.h"
#include "tbb/task.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

void Radix(int byte, int size, int* sourceArr, int* destArr)
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

void RadixSort(int* sourceArr, int size)
{
	int* temp = new int [size];
	Radix(0, size, sourceArr, temp);
	Radix(1, size, temp, sourceArr);
	Radix(2, size, sourceArr, temp);
	Radix(3, size, temp, sourceArr);
	delete[] temp;
}

class EvenSplitter : public tbb::task
{
private:
	int* arr_;
	int* tmp_;
	int size1_;
	int size2_;

public:
	EvenSplitter(int* arr, int* tmp, int size1,
		int size2) : arr_(arr), tmp_(tmp),
		size1_(size1), size2_(size2) {}

	task* execute()
	{
		for (int i = 0; i < size1_; i++)
		{
			tmp_[i] = arr_[i];
		}

		int* mas = arr_ + size1_;

		int a = 0;
		int b = 0;
		int i = 0;
		while ((a < size1_) && (b < size2_))
		{
			if (tmp_[a] <= mas[b])
			{
				arr_[i] = tmp_[a];
				a += 2;
			}
			else
			{
				arr_[i] = mas[b];
				b += 2;
			}
			i += 2;
		}
		if (a == size1_)
			for (int j = b; j < size2_; j += 2, i += 2)
				arr_[i] = mas[j];
		else
			for (int j = a; j < size1_; j += 2, i += 2)
				arr_[i] = tmp_[j];
		return NULL;
	}
};

class OddSplitter : public tbb::task 
{
private:
	int* arr_;
	int* tmp_;
	int size1_;
	int size2_;
public:
	OddSplitter(int* arr, int* tmp, int size1,
		int size2) : arr_(arr), tmp_(tmp),
		size1_(size1), size2_(size2)
	{}
	task* execute()
	{
		for (int i = 1; i < size1_; i += 2)
			tmp_[i] = arr_[i];

		int *mas = arr_ + size1_;
		int a = 1;
		int b = 1;
		int i = 1;
		while ((a < size1_) && (b < size2_))
		{
			if (tmp_[a] <= mas[b])
			{
				arr_[i] = tmp_[a];
				a += 2;
			}
			else
			{
				arr_[i] = mas[b];
				b += 2;
			}
			i += 2;
		}
		if (a == size1_)
			for (int j = b; j < size2_; j += 2, i += 2)
				arr_[i] = mas[j];
		else
			for (int j = a; j < size1_; j += 2, i += 2)
				arr_[i] = tmp_[j];
		return NULL;
	}
};

class SimpleComparator
{
private:
	int *arr;
	int size;
public:
	SimpleComparator(int *_arr, int _size) : arr(_arr),
		size(_size)
	{}

	void operator()(const tbb::blocked_range<int>& r) const
	{
		int begin = r.begin(), end = r.end();

		for (int i = begin; i < end; i++)
			if (arr[2 * i] < arr[2 * i - 1])
			{
				std::swap(arr[2 * i], arr[2 * i - 1]);
			}
	}
};


class ParallelSort :public tbb::task
{
private:
	int* arr;
	int* tmp;
	int size;
	int portion;

public:
	ParallelSort(int* _arr, int* _tmp, int _size,
		int _portion) : arr(_arr), tmp(_tmp),
		size(_size), portion(_portion) {}

	task* execute()
	{
		if (size <= portion)
		{
			RadixSort(arr, size);
		}
		else 
		{
			int s = size / 2 + (size / 2) % 2;
			ParallelSort &sorter1 = *new (allocate_child())
				ParallelSort(arr, tmp, s, portion);
			ParallelSort &sorter2 = *new (allocate_child())
				ParallelSort(arr + s, tmp + s, size - s,
					portion);
			set_ref_count(3);
			spawn(sorter1);
			spawn_and_wait_for_all(sorter2);

			EvenSplitter &splitter1 = *new (allocate_child())
				EvenSplitter(arr, tmp, s, size - s);
			OddSplitter &splitter2 = *new (allocate_child())
				OddSplitter(arr, tmp, s, size - s);
			set_ref_count(3);
			spawn(splitter1);
			spawn_and_wait_for_all(splitter2);


			tbb::parallel_for(tbb::blocked_range<int>(1, (size + 1) / 2),
				SimpleComparator(arr, size));
		}

		return NULL;
	}
};