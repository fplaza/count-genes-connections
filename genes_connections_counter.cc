/*
 * (C) Copyright 2015 Enterome
 */

#include "genes_connections_counter.hh"
#include "fasta_reader.hh"
#include <vector>

const char GenesConnectionsCounter::HEADER_TYPE1_KEYWORD[] = "_revised_";
const char GenesConnectionsCounter::HEADER_TYPE2_KEYWORD[] = "  [gene]  locus=";

boost::unordered_map<std::pair<uint32_t, uint32_t>,uint32_t> GenesConnectionsCounter::count_connections()
{
	boost::unordered_map<std::pair<uint32_t, uint32_t>,uint32_t> genes_connections; 
	genes_connections.rehash(200000000);

	FastaReader fasta_reader(fasta_file_);
	std::string record_header;

	std::string prev_contig_name;
	uint32_t prev_gene_cluster = 0;

	// Read each gene header
	while (fasta_reader.next_record(record_header))
	{
		std::string cur_gene_name;
		std::string cur_contig_name;

		// Extract gene name and its contig
		// Skip it if the header has an unknown format
		if (!extract_gene_infos(record_header, cur_gene_name, cur_contig_name))
			continue;

		// Search in which cluster the gene is
		// Skip it if it is not indexed
		boost::unordered_map<std::string, uint32_t>::const_iterator gene_to_cluster_it = gene_to_cluster_map_.find(cur_gene_name);
		if (gene_to_cluster_it == gene_to_cluster_map_.end())
		{
			continue;
		}

		const uint32_t cur_gene_cluster = gene_to_cluster_it->second;

		if (cur_contig_name == prev_contig_name)
		{
			genes_connections[std::make_pair(prev_gene_cluster, cur_gene_cluster)] += 1;
		}
		else
		{
			prev_contig_name = cur_contig_name;
		}

		prev_gene_cluster = cur_gene_cluster;
	}

	return genes_connections;
}


bool GenesConnectionsCounter::extract_gene_infos(const std::string& record_header, std::string& gene_name, std::string& contig_name)
{
	size_t pos_keyword;

	// First kind of header (american samples)
	pos_keyword = record_header.find(HEADER_TYPE1_KEYWORD);
	if (pos_keyword != std::string::npos)
	{
		const size_t gene_name_beg = 0;
		const size_t gene_name_end = record_header.find('\t');
		gene_name.assign(record_header, gene_name_beg,  gene_name_end-gene_name_beg);

		const size_t contig_name_beg = pos_keyword+sizeof(HEADER_TYPE1_KEYWORD)-1;
		const size_t contig_name_end = record_header.find("_gene", contig_name_beg);
		contig_name.assign(record_header, contig_name_beg,  contig_name_end-contig_name_beg);

		return true;
	}

	// Second kind of header (all others samples)
	pos_keyword = record_header.find(HEADER_TYPE2_KEYWORD);
	if (pos_keyword != std::string::npos)
	{
		const size_t gene_name_beg = 0;
		const size_t gene_name_end = pos_keyword;
		gene_name.assign(record_header, gene_name_beg,  gene_name_end-gene_name_beg);

		const size_t contig_name_beg = gene_name_end+sizeof(HEADER_TYPE2_KEYWORD)-1;
		const size_t contig_name_end = record_header.find(':', contig_name_beg);
		contig_name.assign(record_header, contig_name_beg,  contig_name_end-contig_name_beg);

		return true;
	}

	return false;
};
