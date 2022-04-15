/*
 * @Author: Wang Zongwu
 * @Date: 2022-04-15 16:22:54
 * @LastEditTime: 2022-04-16 01:26:13
 * @LastEditors: Wang Zongwu
 * @Description: Benchmark of multi-thread memcpy for the HW's NCC project
 * @FilePath: /Blog/share/CPP_Proj/project/pthread/memcpy_thread/src/memcpy_thread.cpp
 * @Mail: wangzongwu@sjtu.edu.cn
 * Please ask for permission before quote the code.
 */
#include <iostream>
#include <fstream>
#include <thread>
#include <string.h>
// #include <chrono>
#include <unistd.h>
#include <time.h>
#include <mutex>

// #define DEBUG

using namespace std;

#define ITERNUM 10

class ID
{
private:
	char name[256];
	uint64_t age;
	uint64_t salary;
	uint64_t codeSize;
	uint64_t *missile_launch_code;
	std::mutex my_mutex;
public:
	uint64_t launch_time;
	ID(char *_name, uint64_t _age, uint64_t _salary, uint64_t _len):age(_age), salary(_salary), codeSize(_len), launch_time(0)
	{
		strcpy(name, _name);
		missile_launch_code = (uint64_t *)malloc(codeSize * sizeof(uint64_t));
	}
	void initCode()
	{
		my_mutex.lock();
		#ifdef DEBUG
		cout << this->name << " initialize the launch code!!!" << endl;
		#endif
		for (size_t i = 0; i < codeSize; i++)
		{
			*(this->missile_launch_code + i) = rand() % 100000;
		}
		my_mutex.unlock();
	}
	void resetCode(bool *afterSync)
	{
		*afterSync = false;
		my_mutex.lock();
		#ifdef DEBUG
		cout << this->name << " reset the launch code!!!" << endl;
		#endif
		for (size_t i = 0; i < codeSize; i++)
		{
			this->missile_launch_code[i] = 0;
		}
		my_mutex.unlock();
	}
	void sync(ID *origin, bool *afterSync)
	{
		*afterSync = true;
		my_mutex.lock();
		origin->my_mutex.lock();
		#ifdef DEBUG
		cout << this->name << " copy information from " << origin->name <<endl;
		#endif
		memcpy((void *)this->missile_launch_code, (void *)origin->missile_launch_code, this->codeSize * sizeof(uint64_t));
		origin->my_mutex.unlock();
		my_mutex.unlock();
	}
	void askForLaunch(ID *id_comp)
	{
		my_mutex.lock();
		id_comp->my_mutex.lock();
		#ifdef DEBUG
		cout << this->name << " ask for launch missile from " << id_comp->name << endl;
		#endif
		bool success = true;
		for (size_t i = 0; i < codeSize; i++)
		{
			if (this->missile_launch_code[i] != id_comp->missile_launch_code[i])
			{
				// cout << "Thread id [" << std::this_thread::get_id() << "]: Missile launch code violation!!!" << endl;
				success = false;
				break;
			}
		}
		if (success)
		{
			// TODO: get_id() will incur error with --static option
			// cout << "Thread id [" << std::this_thread::get_id() << "]: " << this->name << " has no violation, missile is launching!!!" << endl;
			#ifdef DEBUG
			cout << this->name << " has no violation, missile is launching!!!" << endl;
			for (int i = 10; i > 0; i--)
			{
				cout << i << "...\n";
			}
			cout << "\nIgnite..." << endl;
			cout << "Departures!!!" << endl;
			#endif
			launch_time++;
		}
		else
		{
			// cout << "Error: Thread id [" << std::this_thread::get_id() << "]: Violation exist, " << this->name << " has no right information, permission denied!!!" << endl;
			#ifdef DEBUG
			cout << this->name << " has no right information, permission denied!!!" << endl;
			#endif
		}
		id_comp->my_mutex.unlock();
		my_mutex.unlock();
	}
	~ID()
	{
		free(missile_launch_code);
	}
};


int main()
{
	srand(time(NULL));
	uint64_t infoSize;
	ID *president;
	ID *substitute;
	// ID *cheater;
	bool Synced = false;
	bool *afterSync = &Synced;


	infoSize = rand() % 512;

	president = new ID((char *)"Trump", 68, 1000000, infoSize);
	substitute = new ID((char *)"Trumpet", 18, 0, infoSize);
	// cheater = new ID((char *)"Biden", 78, 0, infoSize);
	president->initCode();
	substitute->initCode();
	std::thread tobj1(&ID::askForLaunch, substitute, president);
	tobj1.join();
	// usleep(1000);		// sleep for 1ms
	std::thread tobj2(&ID::sync, substitute, president, afterSync);
	tobj2.join();
	// usleep(1);		// sleep for 1ms
	while (!afterSync) {}
	std::thread tobj3(&ID::askForLaunch, substitute, president);
	tobj3.join();
	usleep(1000);		// sleep for 1ms
	std::thread tobj4(&ID::resetCode, president, afterSync);
	tobj4.join();
	// usleep(1000);		// sleep for 1ms
	// TODO: 为什么这边的while会是死循环？
	// while (afterSync) {}
	// 这边tobj5会不会先于tobj4获得锁？
	std::thread tobj5(&ID::askForLaunch, substitute, president);
	tobj5.join();

	if (substitute->launch_time != 1)
	{
		printf("Error: thread error!!!\n");
		return -1;
	}

	free(president);
	free(substitute);
	// free(cheater);
	president = nullptr;
	substitute = nullptr;
	// cheater = nulllptr;

	return 0;
}
