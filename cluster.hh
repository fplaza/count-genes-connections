#ifndef CLUSTER_HH
#define CLUSTER_HH

/*
 * (C) Copyright 2015 Enterome
 */

#include <stdint.h>
#include <vector>
#include <string>
#include <ostream>

struct Cluster
{
	uint32_t id;
	std::vector<std::string> genes;
};

std::ostream& operator<<(std::ostream& out, const Cluster& cluster);

#endif //CLUSTER_HH
