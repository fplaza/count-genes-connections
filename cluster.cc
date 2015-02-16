/*
 * (C) Copyright 2015 Enterome
 */

#include "cluster.hh"

std::ostream& operator<<(std::ostream& out, const Cluster& cluster)
{
	out << ">Cluster " << cluster.id+1 << '\n';

	for (std::vector<std::string>::const_iterator genes_it = cluster.genes.begin(); genes_it < cluster.genes.end(); genes_it++)
		out << *genes_it << '\n';

	return out;
}

