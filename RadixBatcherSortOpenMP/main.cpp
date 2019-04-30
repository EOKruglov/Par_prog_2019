#include "RadixSort.h"
#include <omp.h>
#include "TbbVersionSort.h"



int main()
{
	int size = 16;
	srand(time(NULL));

	std::vector<int> mainArr(size);
	std::vector<int> tmp(size);
	std::vector<int> arr(size);
	int *tbbArray = new int[size];
	int *tbbTmp = new int[size];
	SequentialSort ss;
	OmpSort os;
	
	ArrayFill(mainArr, size);

	for (int i = 0; i < size; i++)
	{
		tbbArray[i] = mainArr[i];
		tbbTmp[i] = mainArr[i];
	}
	tmp = mainArr;
    arr = mainArr;
	double t1, t2, dts, dtp;

	std::cout << "Result for " << size << " elements" << std::endl;

	t1 = omp_get_wtime();
	ss.RadixSort(arr, size);
	t2 = omp_get_wtime();
	dts = t2 - t1;

	sort(tmp.begin(), tmp.end());

	if (CheckResult(arr, tmp))
	{
		std::cout << "Correct result for sequantial version" << std::endl;
	}

	else
	{
		std::cout << "Incorrect result for sequantial version" << std::endl;
	}

	std::cout << "Time = " << dts << " seconds" << std::endl;



	/////////////////////////Omp version starts here////////////////////////////////////////

	arr = mainArr;

	int threadsNum = 4;
	int threadId;
	int mergeNum;
	bool flag = true;
	int coord[2][4];
	if (size % threadsNum != 0)
	{
		flag = false;
	}


#pragma omp parallel num_threads(threadsNum) shared(flag, coord, size, arr) private(threadId)
	{
		threadId = omp_get_thread_num();
		int tail = 0;
		int k = 0;

		if (threadId == 0)
		{
			if (flag)
			{	
					coord[0][0] = 0;
					coord[1][0] = size / threadsNum - 1;
					for (int i = 1; i <= threadsNum - 1; i++)
					{
						coord[0][i] = coord[0][i - 1] + (size / threadsNum);
						coord[1][i] = coord[1][i - 1] + (size / threadsNum);
					}
			}

			else
			{
				tail = threadsNum - (size % threadsNum);
				int newSize = size + tail;
				for (int i = 0; i < tail; i++)
				{
					arr.push_back(0xff);
				}
				coord[0][0] = 0;
				coord[1][0] = newSize / threadsNum - 1;
				for (int i = 1; i <= threadsNum - 1; i++)
				{
					coord[0][i] = coord[0][i - 1] + (newSize / threadsNum);
					coord[1][i] = coord[1][i - 1] + (newSize / threadsNum);
				}

			}

		}
		t1 = omp_get_wtime();
		os.RadixSort(arr, coord[0][threadId], coord[1][threadId]);

#pragma omp barrier
		int m = 1;
		int i = 0;
		for (i; i < threadsNum / 2; i++, m *= 2)
		{
			if ((threadId % (2 * m) == 0) && (threadsNum - threadId > m))
			{

				os.BatcherMerge(arr, coord[0][threadId * m], coord[1][threadId], coord[0][(threadId + 1) * m],
					coord[1][threadId + (int)pow(2, i)]);

				coord[1][threadId] = coord[1][(threadId + 1) * (int)pow(2, i)];
			}

#pragma omp barrier
		}
		t2 = omp_get_wtime();
		if (tail != 0)
		{
			for (int j = 0; j < tail; j++)
			{
				arr.pop_back();
			}
		}
	}
	dtp = t2 - t1;
		if (CheckResult(arr, tmp))
		{
			std::cout << "Correct result for parallel version" << std::endl;
		}
	
		else
		{
			std::cout << "Incorrect result for parallel version" << std::endl;
		}

		std::cout << "Time = " << dtp << " seconds"<< std::endl;
		std::cout << "Acceleration = " << dts / dtp << std::endl;



		/////////////////////////////TBB VERSION//////////////////////////////////////////
		
		tbb::task_scheduler_init init(threadsNum);

		t1 = omp_get_wtime();
		ParallelSort& task = *new(tbb::task::allocate_root()) ParallelSort(tbbArray, tbbTmp, size, size / 100);
		t2 = omp_get_wtime();
		double tbbTime = t2 - t1;

		bool tbbflag = true;
		for (int i = 0; i < size; i++)
		{
			if (tbbArray[i] != tmp[i])
			{
				tbbflag = false;
				break;
			}
		}

		if (tbbflag)
		{
			std::cout << "Correct result for tbb version" << std::endl;
		}
		else
		{
			std::cout << "Incorrect result for tbb version" << std::endl;
		}

		std::cout << "Time = " << tbbTime << std::endl;
		std::cout << "Acceleration = " << dts / tbbTime << std::endl;
		for (int i = 0; i < size; i++)
		{
			std::cout << mainArr[i] << " ";
		}
		std::cout << std::endl;
		for (int i = 0; i < size; i++)
		{
			std::cout << tbbArray[i] <<" ";
		}
		std::cout << std::endl;
	return 0;
}