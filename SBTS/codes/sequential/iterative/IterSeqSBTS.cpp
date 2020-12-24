// Iterative sequential smallest binary tree size
// Fall 2020


#include "IterSeqSBTS.h"


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
// The sequential and iterative algorithm for calculating the small binary size tree
//==================================================================================
int iterative_sequential_sbst(Simplex simplex)
{
	Node *node_ptr 						= createNode(simplex, nullptr, 0);


	while (true)
	{

		//--------------------------------------------------------------------------
		// Create children nodes of the current node (moving forward)
		//--------------------------------------------------------------------------
		if(node_ptr->is_children_created == false)
		{
			createChildNodes(node_ptr);
		}

		bool sbts_calculation_step 		= true;


		//--------------------------------------------------------------------------
		// Move to one of the unseen child for calculating the sbts
		//--------------------------------------------------------------------------
		for(int i = 0; i < node_ptr->children_sbts.size(); ++i)
		{
			auto *children_pair_sbts 	= &node_ptr->children_sbts.at(i);

			if(std::get<0>(*children_pair_sbts) == 0 )
			{
				node_ptr 				= node_ptr->children_ptr_to_node.at(i).first;
				sbts_calculation_step 	= false;
				break;

			} else if((std::get<1>(*children_pair_sbts) == 0) && (std::get<2>(*children_pair_sbts) == false))
			{
				node_ptr 				= node_ptr->children_ptr_to_node.at(i).second;
				sbts_calculation_step 	= false;
				break;

			} // If condition end

		} // For loop end


		//--------------------------------------------------------------------------
		// Update childern sbts
		//--------------------------------------------------------------------------
		if (sbts_calculation_step == false)
		{
		 	continue;

		} else
		{
			if (node_ptr->sbts != 1)
			{
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

				} // For loop end
			}

		} // If condition (sbts_calculation_step) end




		//--------------------------------------------------------------------------
		// Update parent sbts (moving backward)
		//--------------------------------------------------------------------------
		if (node_ptr->parent_node_ptr != nullptr)
		{
			Node *temp_ptr 				= node_ptr->parent_node_ptr;
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

			delete node_ptr;
			node_ptr 					= temp_ptr;
		
		} else 
		{
			break;
		
		} // If condition (node_ptr->parent_node_ptr) end
	
	} // While loop end

	return node_ptr->sbts;// * dimension * (dimension - 1) * 0.5;
}




















