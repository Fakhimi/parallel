// Smallest binary tree size in unit simplex refinement
// Fall 2020

#include <iostream>
#include <stdlib.h>
#include <chrono>

#include "IterParPthSBTS.h"
int thread_num, active_thread_num = 0;
double epsilon 	= 0;
int dimension 	= 0;

pthread_spinlock_t thread_spin_lock;

int main(int argc, char *argv[])
{

	std::cin >> dimension >> epsilon >> thread_num;

	std::cout << "dimension:\t" << dimension << std::endl;
	std::cout << "epsilon:\t" << epsilon << std::endl;
	std::cout << "thread number:\t" << thread_num << std::endl;
	
	auto begin = std::chrono::high_resolution_clock::now();

	Simplex initial_simplex(dimension);

	pthread_spin_init(&thread_spin_lock, PTHREAD_PROCESS_SHARED);

	int smallest_binary_tree_size 	= iterative_parallel_pthread_sbst(initial_simplex);

	auto end 	= std::chrono::high_resolution_clock::now();
	float time 	= static_cast<float>(std::chrono::duration_cast<std::chrono::seconds>(end-begin).count());


	std::cout << "total time:\t" << time <<" (s)" << std::endl; 

	std::cout << "smallest binary tree size:\t" << smallest_binary_tree_size << std::endl;

	return 0;
}
