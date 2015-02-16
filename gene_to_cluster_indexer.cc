/*
 * (C) Copyright 2015 Enterome
 */

#include "gene_to_cluster_indexer.hh"
#include "cluster_reader.hh"

boost::unordered_map<std::string, uint32_t> GeneToClusterIndexer::make_index()
{
	// Count the total number of genes to avoid rehashing
	size_t num_genes = 0;
	Cluster cluster;
	ClusterReader cluster_reader(clusters_file_);
	while(cluster_reader.next_cluster(cluster))
	{
		num_genes += cluster.genes.size();
	}

	boost::unordered_map<std::string, uint32_t> gene_to_cluster_map;
	gene_to_cluster_map.rehash(num_genes);

	cluster_reader.reset();
	while(cluster_reader.next_cluster(cluster))
	{
		for(std::vector<std::string>::const_iterator genes_it = cluster.genes.begin(); genes_it < cluster.genes.end(); genes_it++)
		{
			gene_to_cluster_map[*genes_it] = cluster.id;
		}
	}

	return gene_to_cluster_map;
}
