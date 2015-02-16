/*
 * (C) Copyright 2015 Enterome
 */

#include "cluster_reader.hh"

std::vector<Cluster> ClusterReader::read_all()
{
	std::vector<Cluster> all_clusters;
	all_clusters.reserve(1000000);

	Cluster cluster;
	while(next_cluster(cluster))
		all_clusters.push_back(cluster);

	return all_clusters;
}
