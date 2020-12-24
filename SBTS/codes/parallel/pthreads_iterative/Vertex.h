// Verex
// Fall 2020

#ifndef _VERTEX_H_
#define _VERTEX_H_


#include <iostream>
#include <vector>
#include <cmath>

class Vertex
{
public:
	std::vector<double> coordinates;
	Vertex(const int &dim);
	Vertex(const std::vector<double> &coords);
	~Vertex();

	Vertex &operator=(const Vertex &rhs_vertex);

};

double distance(const std::pair<Vertex, Vertex> &edge);
bool operator==(const Vertex &lhs_vertex, const Vertex &rhs_vertex);

#endif
