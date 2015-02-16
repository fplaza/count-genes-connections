#ifndef GENE_TO_CLUSTER_INDEXER_HH
#define GENE_TO_CLUSTER_INDEXER_HH

/*
 * (C) Copyright 2015 Enterome
 */

#include <string>
#include <stdint.h>
#include <boost/unordered_map.hpp>

class GeneToClusterIndexer
{
	public:
		GeneToClusterIndexer(const std::string& clusters_file);
		boost::unordered_map<std::string, uint32_t> make_index();

	private:
		const std::string clusters_file_;
};

inline GeneToClusterIndexer::GeneToClusterIndexer(const std::string& clusters_file)
	: clusters_file_(clusters_file)
{}

#endif // GENE_TO_CLUSTER_INDEXER_HH
