// Simplex
// Fall 2020

#ifndef _SIMPLEX_H_
#define _SIMPLEX_H_

#include <iostream>
#include <vector>
#include <cmath>

#include "Vertex.h"
#include <tuple> 
#include <unordered_map>
extern double epsilon;

class Simplex
{
private:
	int _dimension 				= 0;
	
public:
	double longest_edge_length 	= -1;
	std::vector<Vertex> vertices;
	std::unordered_map<int, double> edges_length;
	

	Simplex();
	Simplex(const int &dim);
	Simplex(const int &dim, const std::vector<Vertex> &verts);
	~Simplex();
	
	std::vector< std::pair<int, int> > find_longest_edges();
	std::tuple<Simplex, Simplex, bool> bisect(const std::pair<int, int> &edge);
	int generate_key(const int &vertex_first, const int &vertex_second);


};

#endif
