// Iterative sequential smallest binary tree size
// Fall 2020

#ifndef _ITERPARPTHSBTS_H_
#define _ITERPARPTHSBTS_H_

#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>
#include <thread>

#include "Simplex.h"
extern int thread_num, active_thread_num;
extern double epsilon;
extern int dimension;
extern pthread_spinlock_t thread_spin_lock;

class Node
{
public:
	Node();
	~Node();
	Node *parent_node_ptr 			= nullptr;

	Simplex *simplex 				= nullptr;
	
	int sbts 						= 0;
	int child_index_in_parent		= -1;
	bool is_children_created 		= false;
	bool is_done 					= false;
	pthread_spinlock_t spin_lock;

	std::vector<std::pair<int, int>> longest_edges;
	std::vector<std::tuple<int, int, bool>>  children_sbts;
	std::vector<std::pair<Node *, Node *>> children_ptr_to_node;

};

Node *createNode(Simplex simplex, Node *parent_node_ptr, int child_index_in_parent);
void createChildNodes(Node *node_ptr);
std::pair<int, bool> nextChildToProcess(Node *node_ptr);

void* iterative_parallel_pthread_sbst_new_thread(void *argument);
int iterative_parallel_pthread_sbst(Simplex simplex);

#endif
