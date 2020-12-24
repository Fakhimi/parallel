// Simplex
// Fall 2020

#ifndef _SIMPLEX_H_
#define _SIMPLEX_H_

#include <iostream>
#include <vector>
#include <math.h>

#include "Vertex.h"

class Simplex
{
private:
	int _dimension 				= 0;
public:

	std::vector<Vertex> vertices;
	double longest_edge_length 	= -1;
	
	Simplex();
	Simplex(const int &dim);
	Simplex(const int &dim, const std::vector<Vertex> &verts);
	~Simplex();
	
	std::vector< std::pair<int, int> > find_longest_edges();
	std::pair<Simplex, Simplex> bisect(const std::pair<int, int> &edge);

};

#endif
