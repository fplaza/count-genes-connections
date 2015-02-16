#ifndef CLUSTER_READER_HH
#define	CLUSTER_READER_HH

/*
 * (C) Copyright 2015 Enterome
 */

#include "cluster.hh"
#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>

class ClusterReader
{
	private:
		const std::string& cluster_file_;
		std::ifstream stream_;
		std::string line_;

	public:
		ClusterReader(const std::string& cluster_file);
		~ClusterReader();
		void reset();
		bool next_cluster(Cluster& cluster);
		std::vector<Cluster> read_all();
};


inline ClusterReader::ClusterReader(const std::string& cluster_file)
	: cluster_file_(cluster_file), stream_(cluster_file_.c_str()), line_()
{
	line_.reserve(200);
}

inline ClusterReader::~ClusterReader()
{
	stream_.close();
}

inline void ClusterReader::reset()
{
	stream_.close();
	stream_.open(cluster_file_.c_str());
}

inline bool ClusterReader::next_cluster(Cluster& cluster)
{
	if (!stream_.good())
		return false;

	std::getline(stream_, line_);
	if (line_[0] != '>')
	{
		stream_.clear(std::ios_base::failbit);
		return false;
	}
	
	cluster.id = boost::lexical_cast<uint32_t>(line_.substr(9));

	cluster.genes.clear();
	while (stream_.peek() != EOF && stream_.peek() != '>')
	{
		std::getline(stream_, line_);

		const size_t gene_start = line_.find('>')+1;
		const size_t gene_end = line_.find("...", gene_start)-1;
		const std::string& gene = line_.substr(gene_start, gene_end-gene_start+1);
		cluster.genes.push_back(gene);
	}

	return true;
}

#endif // CLUSTER_READER_HH
