// Iterative sequential smallest binary tree size
// Fall 2020


#include "IterParPthSBTS.h"


//==================================================================================
// Node constructor
//==================================================================================
Node::Node()
{
	pthread_spin_init(&spin_lock, PTHREAD_PROCESS_SHARED);
}

//==================================================================================
// Node destructor
//==================================================================================
Node::~Node()
{
	
}

//==================================================================================
// Create parent nodes
//==================================================================================
Node *createNode(Simplex simplex, Node *parent_node_ptr, int child_index_in_parent)
{
	Node *node_ptr						= new Node;
	node_ptr->simplex 					= new Simplex;

	*node_ptr->simplex 					= simplex;
	node_ptr->parent_node_ptr 			= parent_node_ptr;	
	node_ptr->child_index_in_parent 	= child_index_in_parent;

	node_ptr->longest_edges 			= node_ptr->simplex->find_longest_edges();

	return node_ptr;
}


//==================================================================================
// Create all the children nodes of the current node
//==================================================================================
void createChildNodes(Node *node_ptr)
{
	if (node_ptr->simplex->longest_edge_length > epsilon)
	{
		for(size_t edge_it; edge_it < node_ptr->longest_edges.size(); ++edge_it)
		{
			std::tuple<Simplex, Simplex, bool> bisection 	= node_ptr->simplex->bisect(node_ptr->longest_edges.at(edge_it));

			Node *first_node 								= createNode(std::get<0>(bisection), node_ptr, edge_it);
			Node *second_node 								= nullptr;

			if(std::get<2>(bisection) == false)
			{
				second_node 								= createNode(std::get<1>(bisection), node_ptr, edge_it);
			}

			node_ptr->children_ptr_to_node.push_back(std::make_pair(first_node, second_node));
			node_ptr->children_sbts.push_back(std::make_tuple(0, 0, std::get<2>(bisection)));

			if(node_ptr->longest_edges.size() == (dimension * (dimension - 1) / 2) )
			{
				break;
			}
		}

	} else
	{
		node_ptr->sbts 										= 1;
	}

	node_ptr->longest_edges.clear();
	delete node_ptr->simplex;
	node_ptr->is_children_created 							= true;
}



//==================================================================================
// Find the next child node to be processed
//==================================================================================
std::pair<int, bool> nextChildToProcess(Node *node_ptr)
{
	//------------------------------------------------------------------------------	
	// -2: The thread was unsuccessful either the node is busy or it is done
	// true: first node		false: second node
	//------------------------------------------------------------------------------
	std::pair<int, bool> index = std::make_pair(-2, true); 	

	if (0 != pthread_spin_trylock(&node_ptr->spin_lock) )
	{
		//--------------------------------------------------------------------------
		// node is blocked by other threads
		//--------------------------------------------------------------------------
		index.first 			= -1;	
		// std::cout << "Next node is unsuccessful "<<pthread_self() << std::endl;			

	} else
	{
		//--------------------------------------------------------------------------
		// Look for pending child to be processed
		//--------------------------------------------------------------------------
		node_ptr->is_done 		= true;

		for(int i = 0; i < node_ptr->children_sbts.size(); ++i)
		{
			if(std::get<0>(node_ptr->children_sbts.at(i)) == -1)
			{
				node_ptr->is_done 								= false;

			} else if(std::get<0>(node_ptr->children_sbts.at(i)) == 0)
			{
				index.first 									= i;
				std::get<0>(node_ptr->children_sbts.at(i)) 		= -1;
				node_ptr->is_done 								= false;
				break;
			}

			if(std::get<1>(node_ptr->children_sbts.at(i)) == -1)
			{
				node_ptr->is_done 								= false;

			} else if(std::get<1>(node_ptr->children_sbts.at(i)) == 0)
			{
				index.first 									= i;
				index.second 									= false;
				node_ptr->is_done 								= false;
				std::get<1>(node_ptr->children_sbts.at(i)) 		= -1;
				break;
			}

		} // For loop ends

	} // If condition ends (pthread_spin_trylock)

	pthread_spin_unlock(&node_ptr->spin_lock);

	return index;
}



//==================================================================================
// The parallel and iterative algorithm for calculating the small binary size tree with a new thread
//==================================================================================
void* iterative_parallel_pthread_sbst_new_thread(void *argument)
{
	Node *node_ptr 						= (Node *) (argument);
	while (true)
	{
		//--------------------------------------------------------------------------
		// Create children nodes of the current node (moving forward)
		//--------------------------------------------------------------------------
		if(node_ptr->is_children_created == false)
		{
			createChildNodes(node_ptr);
		}

		//--------------------------------------------------------------------------
		// Move to one of the unseen child for calculating the sbts
		//--------------------------------------------------------------------------
		std::pair<int, bool> index 		= nextChildToProcess(node_ptr);


		//--------------------------------------------------------------------------
		// Trying to assign work to a new thread
		//--------------------------------------------------------------------------	
		pthread_spin_trylock(&thread_spin_lock);
		while((index.first >= 0) && (active_thread_num < thread_num))
		{
			active_thread_num++;
			

			pthread_t thread_id;
			Node *temp_ptr 				= nullptr;
			if(index.second == true)
			{
				temp_ptr 				= node_ptr->children_ptr_to_node.at(index.first).first;
			} else
			{
				temp_ptr 				= node_ptr->children_ptr_to_node.at(index.first).second;
			}

			pthread_create(&thread_id, NULL, iterative_parallel_pthread_sbst_new_thread, (void*) temp_ptr );
			pthread_spin_unlock(&thread_spin_lock);
			
			index 						= nextChildToProcess(node_ptr);
			pthread_spin_trylock(&thread_spin_lock);
		}
		pthread_spin_unlock(&thread_spin_lock);


		
		if(index.first == -1) 
		{
			//----------------------------------------------------------------------
			// Node is locked by another thread and look for work in the parent node
			//----------------------------------------------------------------------
			node_ptr 					= node_ptr->parent_node_ptr;
			if (node_ptr == nullptr)
			{
				pthread_spin_trylock(&thread_spin_lock);
				active_thread_num--;
				pthread_spin_unlock(&thread_spin_lock);
				break;
			}

		} else if(index.first >= 0)
		{
			//----------------------------------------------------------------------
			// Could not create a thread or there is a pending child
			//----------------------------------------------------------------------
			if(index.second == true)
			{
				node_ptr 				= node_ptr->children_ptr_to_node.at(index.first).first;
			} else
			{
				node_ptr 				= node_ptr->children_ptr_to_node.at(index.first).second;
			}

			continue;

		} else
		{
			
			if(node_ptr->is_done == false)
			{
				//------------------------------------------------------------------
				// There is at least a child node that is being processed by other thread
				//------------------------------------------------------------------
				if(node_ptr->parent_node_ptr == nullptr) 			
				{
					//--------------------------------------------------------------
					// This is the root node and there is no work upwards
					//--------------------------------------------------------------
					pthread_spin_trylock(&thread_spin_lock);
					active_thread_num--;
					pthread_spin_unlock(&thread_spin_lock);
					break;
				
				} else
				{
					//--------------------------------------------------------------
					// Look for work in the parent node
					//--------------------------------------------------------------
					node_ptr 			= node_ptr->parent_node_ptr;
				}

			} else
			{
				//------------------------------------------------------------------
				// Update sbts of the current node if the node is not a leaf node
				//------------------------------------------------------------------
				if (node_ptr->sbts != 1)
				{
					pthread_spin_trylock(&node_ptr->spin_lock);
					node_ptr->sbts 				= std::get<0>(node_ptr->children_sbts.at(0)) +
												  std::get<1>(node_ptr->children_sbts.at(0)) + 1;

					for(int i = 1; i < node_ptr->children_sbts.size(); ++i)
					{
						int binary_tree_size 	= std::get<0>(node_ptr->children_sbts.at(i)) +
												  std::get<1>(node_ptr->children_sbts.at(i)) + 1;

						if (binary_tree_size < node_ptr->sbts)
						{
							node_ptr->sbts 		= binary_tree_size;
						}

					} 
					node_ptr->is_done			= true;

					pthread_spin_unlock(&node_ptr->spin_lock);
				
				} 

				//------------------------------------------------------------------
				// Update parent sbts (moving backward)
				//------------------------------------------------------------------
				if (node_ptr->parent_node_ptr != nullptr)
				{
					pthread_spin_trylock(&node_ptr->spin_lock);
					Node *temp_ptr 				= node_ptr->parent_node_ptr;
					
					node_ptr->is_done			= true;

					pthread_spin_trylock(&temp_ptr->spin_lock);
					int ind 					= node_ptr->child_index_in_parent;

					if(temp_ptr->children_ptr_to_node.at(ind).first == node_ptr)
					{
						std::get<0>(temp_ptr->children_sbts.at(ind) ) = node_ptr->sbts;

					} 

					if (std::get<2>(temp_ptr->children_sbts.at(ind)) == true)
					{
						std::get<1>(temp_ptr->children_sbts.at(ind) ) = std::get<0>(temp_ptr->children_sbts.at(ind) );

					} else if(temp_ptr->children_ptr_to_node.at(ind).second == node_ptr)
					{
						std::get<1>(temp_ptr->children_sbts.at(ind) ) = node_ptr->sbts;
					}
					pthread_spin_unlock(&temp_ptr->spin_lock);
		
					pthread_spin_unlock(&node_ptr->spin_lock);
					delete node_ptr;
					node_ptr 					= temp_ptr;

					
				
				} else 
				{
					pthread_spin_trylock(&thread_spin_lock);
					active_thread_num--;
					pthread_spin_unlock(&thread_spin_lock);
					// std::cout << pthread_self() << " did the last job" << std::endl;
					break;
				
				} // If condition (node_ptr->parent_node_ptr) end

			} // If condition ends (node_ptr->is_done)

		} // If condition ends (index.first)

	} // While loop end 

}



//==================================================================================
// The parallel and iterative algorithm for calculating the small binary size tree
//==================================================================================
int iterative_parallel_pthread_sbst(Simplex simplex)
{
	Node *node_ptr 						= createNode(simplex, nullptr, 0);
	active_thread_num 					= 1;



	iterative_parallel_pthread_sbst_new_thread(node_ptr);

	pthread_spin_trylock(&thread_spin_lock);
	active_thread_num++;
	pthread_spin_unlock(&thread_spin_lock);

	pthread_spin_trylock(&thread_spin_lock);
	while (active_thread_num != 1)
	{
		pthread_spin_unlock(&thread_spin_lock);
		std::this_thread::sleep_for(std::chrono::seconds(1));

		pthread_spin_trylock(&thread_spin_lock);
	}
	pthread_spin_unlock(&thread_spin_lock);

	return node_ptr->sbts;
}














