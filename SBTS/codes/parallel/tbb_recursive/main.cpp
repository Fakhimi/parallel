// Smallest binary tree size in unit simplex refinement
// Fall 2020

#include <iostream>
#include <stdlib.h>
#include <chrono>

#include "tbb/task_scheduler_init.h"
#include "RecParTBBSBTS.h"


int main(int argc, char *argv[])
{

	int dimension, thread_num;
	double epsilon, cut_off; 

	std::cin >> dimension >> epsilon >> cut_off >> thread_num;

	std::cout << "dimension:\t" << dimension << std::endl;
	std::cout << "epsilon:\t" << epsilon << std::endl;
	std::cout << "cut-off:\t" << cut_off << std::endl;
	std::cout << "thread number:\t" << thread_num << std::endl;

	auto begin = std::chrono::high_resolution_clock::now();

	tbb::task_scheduler_init init(thread_num);

	Simplex initial_simplex(dimension);

	

	int smallest_binary_tree_size 	= recursive_parallel_tbb_sbst(initial_simplex, epsilon, cut_off);
	
	auto end 	= std::chrono::high_resolution_clock::now();
	float time 	= static_cast<float>(std::chrono::duration_cast<std::chrono::seconds>(end-begin).count());

	std::cout << "total time:\t" << time <<" (s)" << std::endl; 

	std::cout << "smallest binary tree size:\t" << smallest_binary_tree_size << std::endl;

	return 0;
}
