// Vertex
// Fall 2020

#include "Vertex.h"


//===========================================================================
// Constructor of Vertex
//===========================================================================
Vertex::Vertex(const std::vector<double> & coords)
{

	for(double elm: coords)
	{
		coordinates.push_back(elm);
	}
}

//===========================================================================
// Constructor of Vertex in specific dimension
//===========================================================================
Vertex::Vertex(const int &dim)
{
	for(size_t i = 0; i < dim; ++i)
	{
		coordinates.push_back(0.0);
	}
}


//===========================================================================
// Destructor of Vertex 
//===========================================================================
Vertex::~Vertex()
{
	
}


//===========================================================================
// Calculating the distance between two vertices
//===========================================================================
double distance(const std::pair<Vertex, Vertex> &edge)
{
	double distance_value = 0.0;
	for(size_t i = 0; i < edge.first.coordinates.size(); ++i)
	{
		distance_value += pow(edge.first.coordinates.at(i) - edge.second.coordinates.at(i), 2.0);
	}
	
	return sqrt(distance_value);
}



//===========================================================================
// Check whether two vertices are the same
//===========================================================================
bool operator==(const Vertex &lhs_vertex, const Vertex &rhs_vertex)
{
	bool equal = true;

	for (size_t i = 0; i < lhs_vertex.coordinates.size(); ++i)
	{
		if (lhs_vertex.coordinates.at(i) != rhs_vertex.coordinates.at(i))
		{
			equal = false;
			break;
		}
	}
	
	return equal;
}



//===========================================================================
// Make a copy of vertex
//===========================================================================
Vertex &Vertex::operator=(const Vertex &rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	for(size_t i = 0; i < rhs.coordinates.size(); ++i)
	{
		this->coordinates.at(i) = rhs.coordinates.at(i);
	}

	return *this;
}