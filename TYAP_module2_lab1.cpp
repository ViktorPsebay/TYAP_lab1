#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
using namespace std;

mutex mtx;

void new_matrix_null(double** A, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		A[i] = new double[n];
		for (int j = 0; j < m; j++)
		{
			A[i][j] = 0;
		}
	}
}

void new_matrix_random(double** A, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		A[i] = new double[n];
		for (int j = 0; j < m; j++)
		{
			A[i][j] = rand()%1000;
		}
	}
}

double** mul_matrix(double** mat1, double** mat2, int n)
{
	mtx.lock();
	double** res = new double*[n];
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

void mul_matrix2(double** mat1, double** mat2, double** mat3, int n)
{
	this_thread::sleep_for(chrono::milliseconds(1000));
	while (i < n)
	{
		this_thread::get_id();
		
		mtx.lock();
		if (i >= n)
			break;
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

int main()
{
	setlocale(LC_ALL, "Russian");
	int n = 10;
	double** A = new double*[n];
	new_matrix_random(A, n,n);

	double** B = new double*[n];
	new_matrix_random(B, n, n);

	double** C = new double*[n];
	new_matrix_null(C, n, n);

	thread th1(mul_matrix2, A, B, C, n);
	thread th2(mul_matrix2, A, B, C, n);
	th1.join();
	th2.join();
	th1.detach();
	th2.detach();
	return 0;
}