// Simplex
// Fall 2020


#include "Simplex.h"


//===========================================================================
// Constructor of Simplex default version (without having coordinates)
//===========================================================================
Simplex::Simplex(const int &dim)
	: _dimension{dim}
{
	std::vector<double> coords(_dimension, 0.0);

	for(size_t i = 0; i < _dimension; ++i)
	{
		coords.at(i) 	=  sqrt(2.0) / 2;
		vertices.push_back(Vertex(coords));
		coords.at(i) 	= 0.0;
	}

}


//===========================================================================
// Constructor of Simplex with having coordinates
//===========================================================================
Simplex::Simplex(const int &dim, const std::vector<Vertex> &verts)
	: _dimension{dim}, vertices{verts}
{
	
}


//===========================================================================
// Destructor of Simplex 
//===========================================================================
Simplex::~Simplex()
{
	
}




//===========================================================================
// Finding the longest edges
//===========================================================================
std::vector< std::pair<int, int> > Simplex::find_longest_edges()
{
	std::vector< std::pair<int, int> > longest_edges;

	for(int i = 0; i < _dimension - 1; ++i)
	{
		for(int j = i + 1; j < _dimension; ++j)
		{
			std::pair<Vertex, Vertex> edge 	= {vertices.at(i), vertices.at(j)};
			double edge_length 				= distance(edge);

			if (longest_edge_length == edge_length)
			{
				longest_edges.push_back(std::make_pair(i, j));

			} else if (longest_edge_length < edge_length)
			{
				longest_edges.clear();
				longest_edges.push_back(std::make_pair(i, j));
				longest_edge_length			= edge_length;
			}
		}
	}

	return longest_edges;
}



//===========================================================================
// Bisectecion of simplex based on specific edge
//===========================================================================
std::pair<Simplex, Simplex> Simplex::bisect(const std::pair<int, int> &edge)
{
	Vertex vertex_1 	= this->vertices.at(edge.first);
	Vertex vertex_2 	= this->vertices.at(edge.second);
	std::vector<double> coords(_dimension, 0.0);

	for(size_t i = 0; i < _dimension; ++i)
	{
		coords.at(i) 	= (vertex_1.coordinates.at(i) + vertex_2.coordinates.at(i)) / 2.0;
	}

	Vertex middle(coords);

	Simplex first_simplex(_dimension, this->vertices);
	first_simplex.vertices.at(edge.first) 		= middle;

	Simplex second_simplex(_dimension, this->vertices);
	second_simplex.vertices.at(edge.second) 	= middle;

	return std::make_pair(first_simplex, second_simplex);

}


