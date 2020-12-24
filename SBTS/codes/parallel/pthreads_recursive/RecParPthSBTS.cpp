// Recursive parallel implementation using Phread for the smallest binary tree size problem
// Fall 2020


#include "RecParPthSBTS.h"



//==================================================================================
// Recursive sequential smallest binary tree size
//==================================================================================
int recursive_sequential_sbst(Simplex simplex)
{
	int smallest_binary_tree_size 					= 0;

	std::vector< std::pair<int, int> > longest_edges;

	longest_edges = simplex.find_longest_edges();


	if (simplex.longest_edge_length > epsilon)
	{
		smallest_binary_tree_size 					= 2147483647;
		for(auto edge: longest_edges)
		{
			std::pair<Simplex, Simplex> bisection 	= simplex.bisect(edge);
			int first_binary_tree_size				= recursive_sequential_sbst(bisection.first);
			int second_binary_tree_size				= recursive_sequential_sbst(bisection.second);
			int binary_tree_size 					= first_binary_tree_size + second_binary_tree_size;

			if (binary_tree_size < smallest_binary_tree_size)
			{
				smallest_binary_tree_size 			= binary_tree_size;
			}

		}
	}
	return smallest_binary_tree_size + 1;
}


//=================================================================================
// Recursive parallel TBB smallest binary tree size
//=================================================================================
void* recursive_parallel_pthread_sbst(void *argument)
{
	Data *data_arg_ptr 									= (Data *) (argument);

	int smallest_binary_tree_size 						= 0;
	
	std::vector< std::pair<int, int> > longest_edges = data_arg_ptr->simplex.find_longest_edges();


	if (data_arg_ptr->simplex.longest_edge_length > epsilon)
	{

		if(data_arg_ptr->simplex.longest_edge_length < cut_off)
		{
			smallest_binary_tree_size 					= recursive_sequential_sbst(data_arg_ptr->simplex) - 1;			
		
		} else
		{

			int size 									= longest_edges.size();

			bool *first_flag							= new bool [size];
			bool *second_flag							= new bool [size];

			pthread_t first_id [size];
			pthread_t second_id [size];

			Data **data_arg_ptr_first 					= new Data* [size];
			Data **data_arg_ptr_second 					= new Data* [size];

			//----------------------------------------------------------------------
			// Calculating the smallest_binary_tree_size
			//----------------------------------------------------------------------
			for(size_t i = 0; i < size; ++i)
			{
				std::pair<Simplex, Simplex> bisection 	= data_arg_ptr->simplex.bisect(longest_edges.at(i));

				data_arg_ptr_first[i]					= new Data;
				data_arg_ptr_second[i]					= new Data;
				
				//------------------------------------------------------------------
				// Calculating first_binary_tree_size
				//------------------------------------------------------------------				
				data_arg_ptr_first[i]->simplex 			= bisection.first;

				pthread_spin_trylock(&spinlock);
				if(active_thread_num < thread_num)
				{
					active_thread_num++;
					pthread_spin_unlock(&spinlock);
					first_flag[i]							= true;

					data_arg_ptr_first[i]->is_new_thread	= true;

					pthread_create(&first_id[i], NULL, recursive_parallel_pthread_sbst, (void*) data_arg_ptr_first[i] );
					
					

				} else
				{
					pthread_spin_unlock(&spinlock);

					first_flag[i]							= false;

					data_arg_ptr_first[i]->is_new_thread	= false;
					
					recursive_parallel_pthread_sbst((void*) data_arg_ptr_first[i]) ;
				
				}


				//------------------------------------------------------------------
				// Calculating second_binary_tree_size
				//------------------------------------------------------------------
				data_arg_ptr_second[i]->simplex 			= bisection.second;

				
				pthread_spin_trylock(&spinlock);
				if((active_thread_num < thread_num) && (i != size - 1) )
				{
					active_thread_num++;
					pthread_spin_unlock(&spinlock);
					second_flag[i] 							= true;

					data_arg_ptr_second[i]->is_new_thread	= true;

					pthread_create(&second_id[i], NULL, recursive_parallel_pthread_sbst, (void*) data_arg_ptr_second[i]);

				

				} else
				{
					pthread_spin_unlock(&spinlock);

					second_flag[i]							= false;
					
					data_arg_ptr_second[i]->is_new_thread	= false;

					recursive_parallel_pthread_sbst((void*) data_arg_ptr_second[i]);
					
				}

			} // For loop termination	

			
			//----------------------------------------------------------------------
			// Flag conditions for retrieving the results of new spawned threads
			//----------------------------------------------------------------------
			smallest_binary_tree_size 						= 2147483647;

			for(size_t i = 0; i < size; ++i)
			{
				if(data_arg_ptr_first[i]->is_new_thread	== true)
				{
					pthread_join(first_id[i], NULL);
					pthread_spin_trylock(&spinlock);
					active_thread_num--;
					pthread_spin_unlock(&spinlock);
				}

				if(data_arg_ptr_second[i]->is_new_thread	== true)
				{
					pthread_join(second_id[i], NULL);
					pthread_spin_trylock(&spinlock);
					active_thread_num--;
					pthread_spin_unlock(&spinlock);
				}

				int binary_tree_size 						= data_arg_ptr_first[i]->result + data_arg_ptr_second[i]->result;

				if (binary_tree_size < smallest_binary_tree_size)
				{
					smallest_binary_tree_size 				= binary_tree_size;
				}

				delete data_arg_ptr_first[i];
				delete data_arg_ptr_second[i];

			} // For loop termination
			

			delete [] first_flag;
			delete [] second_flag;

			delete [] data_arg_ptr_first;
			delete [] data_arg_ptr_second;


		} // Cut-off condition

	} // Edge size checking condition

	//------------------------------------------------------------------------------
	// Conditional return for new thread and the existing thread
	//------------------------------------------------------------------------------
	
	data_arg_ptr->result 			= smallest_binary_tree_size + 1;

	if (data_arg_ptr->is_new_thread == true )
	{
		pthread_exit(0);
	} 

	// std::cout << pthread_self() << " " << data_arg_ptr->result << std::endl;


}
	


