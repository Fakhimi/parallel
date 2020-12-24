// Iterative sequential smallest binary tree size
// Fall 2020

#ifndef _ITERSEQSBTS_H_
#define _ITERSEQSBTS_H_

#include <iostream>
#include<vector>

#include "Simplex.h"
extern double epsilon;
extern int dimension;

class Node
{
public:
	Node *parent_node_ptr 						= nullptr;

	Simplex *simplex 							= nullptr;
	
	int sbts 									= 0;
	int child_index_in_parent					= -1;
	bool is_children_created 					= false;

	std::vector<std::pair<int, int>> longest_edges;
	std::vector<std::tuple<int, int, bool>>  children_sbts;
	std::vector<std::pair<Node *, Node *>> children_ptr_to_node;

};

Node *createNode(Simplex simplex, Node *parent_node_ptr, int child_index_in_parent);
void createChildNodes(Node *node_ptr);
int iterative_sequential_sbst(Simplex simplex);

#endif
