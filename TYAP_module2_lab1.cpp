#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
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

int i = 0;
int j = 0;
int k = 0;

void mul_matrix2(int** mat1, int** mat2, int** mat3, int n)
{
	this_thread::sleep_for(chrono::milliseconds(1000));
	while (i < n)
	{
		this_thread::get_id();
		
		mtx.lock();
		if (i >= n)
		{
			mtx.unlock(); break;
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
		mtx.unlock();
	}
	
}

void show_matrix(int** A, int n, int m)
{
	for (int i = 0; i < n; ++i)
	{
		//cout << A[i] << endl << endl;
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

	thread th1(mul_matrix2, A, B, C, n);
	thread th2(mul_matrix2, A, B, C, n);

	th1.join();
	th2.join();

	show_matrix(C, n, n);

	//th1.detach();
	//th2.detach();
	//int a = 1;
	//std::thread thr(threadFunction, std::ref(a));
	//thr.join();
	//std::cout << a << std::endl;
	return 0;
}