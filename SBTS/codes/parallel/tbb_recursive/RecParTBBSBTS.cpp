// Recursive sequential smallest binary tree size
// Fall 2020


#include "RecParTBBSBTS.h"


//===========================================================================
// Recursive sequential smallest binary tree size
//===========================================================================
int recursive_sequential_sbst(Simplex simplex, double epsilon)
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
			int first_binary_tree_size				= recursive_sequential_sbst(bisection.first, epsilon);
			int second_binary_tree_size				= recursive_sequential_sbst(bisection.second, epsilon);
			int binary_tree_size 					= first_binary_tree_size + second_binary_tree_size;

			if (binary_tree_size < smallest_binary_tree_size)
			{
				smallest_binary_tree_size 			= binary_tree_size;
			}

		}
	}
	return smallest_binary_tree_size + 1;
}


//===========================================================================
// Recursive parallel TBB smallest binary tree size
//===========================================================================
int recursive_parallel_tbb_sbst(Simplex simplex, double epsilon, double cut_off)
{
	int smallest_binary_tree_size 					= 0;

	std::vector< std::pair<int, int> > longest_edges;

	longest_edges = simplex.find_longest_edges();


	if (simplex.longest_edge_length > epsilon)
	{
		if(simplex.longest_edge_length < cut_off)
		{
			smallest_binary_tree_size 					= recursive_sequential_sbst(simplex, epsilon) - 1;
			
		} else
		{
			
			smallest_binary_tree_size 					= 2147483647;
			for(auto edge: longest_edges)
			{
				std::pair<Simplex, Simplex> bisection 	= simplex.bisect(edge);
				int first_binary_tree_size, second_binary_tree_size;

				tbb::task_group g;
				g.run([&]{first_binary_tree_size 	= recursive_parallel_tbb_sbst(bisection.first, epsilon, cut_off); });
				g.run([&]{second_binary_tree_size 	= recursive_parallel_tbb_sbst(bisection.second, epsilon, cut_off); });
				g.wait();

				int binary_tree_size 				= first_binary_tree_size + second_binary_tree_size;
				
				if (binary_tree_size < smallest_binary_tree_size)
				{
					smallest_binary_tree_size 			= binary_tree_size;
				}

			}

		}
	}

	return smallest_binary_tree_size + 1;

}

