#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
using namespace std;

mutex mtx;
bool flag = true;

void file(bool banch, int i = 0)
{
	mtx.lock();
	if (banch)
	{
		int a;
		a = rand();
		ofstream output("Text.txt");
		if (output.is_open())
		{
			output << a;
			output.close();
		}
		cout << this_thread::get_id() << "\t\tFirstThread\t\n";
	}
	else
	{
		int b = rand();
		string line;
		if (b % 2)
		{
			cout << i << ". X - " << this_thread::get_id() << "\t\Thread\t" << "\n";
		}
		else cout << i << ". Y - " << this_thread::get_id() << "\t\Thread\t" << "\n";
		//ifstream input("Text.txt");
		//if (input.is_open())
		//{
		//	getline(input, line);
		//	//b = stoi(line);
		//	
		//	input.close();
		//}
	}
	mtx.unlock();
}

void ran()
{
	int a;
	a = rand();
	while (flag)
	{
		this_thread::sleep_for(chrono::milliseconds(1000));
		file(true);
		cout << this_thread::get_id() << "\t\FirstThread\t" << "\n";
	}
}

void PrintXorY()
{
	int b;
	for (int i = 1; i != 0; i++)
	{
		this_thread::sleep_for(chrono::milliseconds(999));
		file(false, i);
		string line;
		//ifstream input("Text.txt");
		//input.close();
		if (i == 100) flag = false;
	}
	flag = false;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	thread th1(PrintXorY);
	thread th2(PrintXorY);
	th1.join();
	th2.join();
	return 0;
}