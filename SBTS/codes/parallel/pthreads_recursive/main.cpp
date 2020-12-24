// Smallest binary tree size in unit simplex refinement
// Fall 2020

#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <thread>

#include "RecParPthSBTS.h"

int thread_num, active_thread_num;
double epsilon, cut_off;
pthread_spinlock_t spinlock;


int main(int argc, char *argv[])
{

	int dimension; 

	std::cin >> dimension >> epsilon >> cut_off >> thread_num;

	std::cout << "dimension:\t" << dimension << std::endl;
	std::cout << "epsilon:\t" << epsilon << std::endl;
	std::cout << "cut-off:\t" << cut_off << std::endl;
	std::cout << "thread number:\t" << thread_num << std::endl;

	active_thread_num 					= 1;
	
	auto begin = std::chrono::high_resolution_clock::now();

	Simplex initial_simplex(dimension);

	Data *data_arg_ptr 					= new Data();
	data_arg_ptr->simplex 				= initial_simplex;
	data_arg_ptr->is_new_thread 		= false;


	pthread_spin_init(&spinlock, PTHREAD_PROCESS_SHARED);

	recursive_parallel_pthread_sbst((void*) data_arg_ptr);
	
	// pthread_spin_trylock(&spinlock);
	// while(active_thread_num > 1)
	// {
	// 	pthread_spin_unlock(&spinlock);
	// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	// 	pthread_spin_trylock(&spinlock);

	// }
	// pthread_spin_unlock(&spinlock);

	auto end 	= std::chrono::high_resolution_clock::now();
	float time 	= static_cast<float>(std::chrono::duration_cast<std::chrono::seconds>(end-begin).count());




	std::cout << "total time:\t" << time <<" (s)" << std::endl; 

	std::cout << "smallest binary tree size:\t" << data_arg_ptr->result << std::endl;

	pthread_spin_destroy(&spinlock);
	delete data_arg_ptr;

	return 0;
}
