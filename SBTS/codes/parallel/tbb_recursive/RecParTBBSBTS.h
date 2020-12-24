// Recursive Parallel using TBB for solving the smallest binary tree size problem
// Fall 2020

#ifndef _RECPARTBBSBTS_H_
#define _RECPARTBBSBTS_H_

#include <iostream>
#include<vector>

#include "Simplex.h"
#include <tbb/task_group.h>

int recursive_sequential_sbst(Simplex initial_simplex, double epsilon);
int recursive_parallel_tbb_sbst(Simplex initial_simplex, double epsilon, double cut_off);

#endif
