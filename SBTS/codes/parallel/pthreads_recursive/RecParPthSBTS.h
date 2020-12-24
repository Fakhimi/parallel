// Recursive sequential smallest binary tree size
// Fall 2020

#ifndef _RECSEQSBTS_H_
#define _RECSEQSBTS_H_

#include <iostream>
#include<vector>

#include "Simplex.h"
#include <pthread.h>
#include <chrono>
#include <thread>


//==================================================================================
// Global variables
//==================================================================================
extern int thread_num, active_thread_num;
extern double epsilon, cut_off;
extern pthread_spinlock_t spinlock;
// extern pthread_t initial_thread;


//==================================================================================
// Class for importing variables
//==================================================================================
class Data
{
public:
	Simplex simplex;
	bool is_new_thread;
	int result = 0;
};


//==================================================================================
// Recursive smallest binary size tree algorithms
//==================================================================================
int recursive_sequential_sbst(Simplex simplex);
void* recursive_parallel_pthread_sbst(void *argument);

#endif
