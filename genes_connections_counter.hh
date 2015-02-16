#ifndef GENES_CONNECTIONS_COUNTER_HH
#define GENES_CONNECTIONS_COUNTER_HH

/*
 * (C) Copyright 2015 Enterome
 */

#include <boost/unordered_map.hpp>

class GenesConnectionsCounter
{
	public:
		inline GenesConnectionsCounter(const boost::unordered_map<std::string,uint32_t>& gene_to_cluster_map, const std::string fasta_file);
		boost::unordered_map<std::pair<uint32_t, uint32_t>,uint32_t> count_connections();

	private:
		static const char HEADER_TYPE1_KEYWORD[];
		static const char HEADER_TYPE2_KEYWORD[];
		bool extract_gene_infos(const std::string& gene_header, std::string& gene_name, std::string& gene_contig_name);
		const boost::unordered_map<std::string,uint32_t>& gene_to_cluster_map_;
		const std::string fasta_file_;
};

inline GenesConnectionsCounter::GenesConnectionsCounter(const boost::unordered_map<std::string,uint32_t>& gene_to_cluster_map, const std::string fasta_file)
	: gene_to_cluster_map_(gene_to_cluster_map), fasta_file_(fasta_file)
{};

#endif // GENES_CONNECTIONS_COUNTER_HH
