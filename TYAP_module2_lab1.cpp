#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <condition_variable>
using namespace std;

mutex mtx;

void new_matrix_null(int** A, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		A[i] = new int[n];
		for (int j = 0; j < m; j++)
		{
			A[i][j] = 0;
		}
	}
}

void new_matrix_random(int** A, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		A[i] = new int[n];
		for (int j = 0; j < m; j++)
		{
			A[i][j] = rand()%1000;
		}
	}
}

int** mul_matrix(int** mat1, int** mat2, int n)
{
	
	int** res = new int*[n];
	new_matrix_null(res, n, n);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			for (int k = 0; k < n; ++k)
				res[i][j] += mat1[i][k] * mat2[k][j];
	return res;
}
timed_mutex t_mtx;

condition_variable wait_other_threads1;
condition_variable_any wait_other_threads;


//unique_lock<timed_mutex> firstGuard(t_mtx, defer_lock);


unsigned short c_SpecialItem = numeric_limits<unsigned short>::max();

int i = 0;
int j = 0;
int k = 0;
int numThreads = 0;

void mul_matrix2(int** mat1, int** mat2, int** mat3, int n)
{
	if (numThreads < 3)
	{		
		numThreads++;
		mtx.lock();
		wait_other_threads.wait(mtx);
		mtx.unlock();
	}
	else
		wait_other_threads.notify_all();
	
	this_thread::sleep_for(chrono::milliseconds(1000));
	while (i < n)
	{
		this_thread::get_id();
		lock_guard<mutex> firstGuard(mtx);
		//mtx.lock();
		if (i >= n)
		{
			//mtx.unlock(); 
			break;
		}
		mat3[i][j] += mat1[i][k] * mat2[k][j];
		k++;
		if (k >= n)
		{
			k = 0; j++;
		}
		if (j >= n)
		{
			j = 0; i++;
		}
		//mtx.unlock();
	}
	
}

void show_matrix(int** A, int n, int m)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			cout << A[i][j] << " ";
		}
		cout << endl;
	}

}

void threadFunction(int &a)
{
	a++;
}

int main()
{
	setlocale(LC_ALL, "Russian");
		
	int n = 10;
	int** A = new int*[n];
	new_matrix_random(A, n,n);

	int** B = new int*[n];
	new_matrix_random(B, n, n);

	int** C = new int*[n];
	new_matrix_null(C, n, n);
	int  num_of_thread = _Thrd_hardware_concurrency();
	/*vector<thread> threads;
	int num_ths = 2;*/
	auto begin = std::chrono::steady_clock::now();
	vector <thread> th_vec;
	/*for (int i = 0; i < 4; i++)
	{
		th_vec.push_back(thread(mul_matrix2, A, B, C));
	}
	for (int i = 0; i < 4; i++)
	{
		th_vec[i].join();
	}*/
	thread th1(mul_matrix2, A, B, C, n);
	thread th2(mul_matrix2, A, B, C, n);
	thread th3(mul_matrix2, A, B, C, n);
	thread th4(mul_matrix2, A, B, C, n);
	
	th1.join();
	th2.join();
	th3.join();
	th4.join();
	auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	std::cout << "The time: " << elapsed_ms.count() << " ms\n";
	show_matrix(C, n, n);

	return 0;
}