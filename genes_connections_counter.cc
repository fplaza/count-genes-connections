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

	std::vector<uint32_t> clusters_in_cur_contig;
	clusters_in_cur_contig.reserve(10);
	std::string cur_contig;

	// Read each gene header
	while (fasta_reader.next_record(record_header))
	{
		std::string gene_name;
		std::string gene_contig_name;

		// Extract gene name and its contig
		// Skip it if the header has an unknown format
		if (!extract_gene_infos(record_header, gene_name, gene_contig_name))
			continue;

		// Search in which cluster the gene is
		// Skip it if it is not indexed
		boost::unordered_map<std::string, uint32_t>::const_iterator gene_to_cluster_it = gene_to_cluster_map_.find(gene_name);
		if (gene_to_cluster_it == gene_to_cluster_map_.end())
		{
			continue;
		}

		const uint32_t gene_cluster = gene_to_cluster_it->second;

		// new contig!
		if (gene_contig_name != cur_contig)
		{
			// update connections of genes of the previous contig
			std::sort(clusters_in_cur_contig.begin(), clusters_in_cur_contig.end());
			for (size_t i = 0; i < clusters_in_cur_contig.size(); i++)
			{
				for (size_t j = i+1; j < clusters_in_cur_contig.size(); j++)
				{
					genes_connections[std::make_pair(clusters_in_cur_contig[i], clusters_in_cur_contig[j])] += 1;
				}
			}

			cur_contig = gene_contig_name;
			clusters_in_cur_contig.clear();
		}
		clusters_in_cur_contig.push_back(gene_cluster);
	}

	// TODO: find a way to deal with the last contig without duplicating the code
	std::sort(clusters_in_cur_contig.begin(), clusters_in_cur_contig.end());
	for (size_t i = 0; i < clusters_in_cur_contig.size(); i++)
	{
		for (size_t j = i+1; j < clusters_in_cur_contig.size(); j++)
		{
			genes_connections[std::make_pair(clusters_in_cur_contig[i], clusters_in_cur_contig[j])] += 1;
		}
	}


	return genes_connections;
}


bool GenesConnectionsCounter::extract_gene_infos(const std::string& gene_header, std::string& gene_name, std::string& gene_contig_name)
{
	size_t pos_keyword;

	// First kind of header (american samples)
	pos_keyword = gene_header.find(HEADER_TYPE1_KEYWORD);
	if (pos_keyword != std::string::npos)
	{
		const size_t gene_name_beg = 0;
		const size_t gene_name_end = gene_header.find('\t');
		gene_name.assign(gene_header, gene_name_beg,  gene_name_end-gene_name_beg);

		const size_t gene_contig_name_beg = pos_keyword+sizeof(HEADER_TYPE1_KEYWORD)-1;
		const size_t gene_contig_name_end = gene_header.find("_gene", gene_contig_name_beg);
		gene_contig_name.assign(gene_header, gene_contig_name_beg,  gene_contig_name_end-gene_contig_name_beg);

		return true;
	}

	// Second kind of header (all others samples)
	pos_keyword = gene_header.find(HEADER_TYPE2_KEYWORD);
	if (pos_keyword != std::string::npos)
	{
		const size_t gene_name_beg = 0;
		const size_t gene_name_end = pos_keyword;
		gene_name.assign(gene_header, gene_name_beg,  gene_name_end-gene_name_beg);

		const size_t gene_contig_name_beg = gene_name_end+sizeof(HEADER_TYPE2_KEYWORD)-1;
		const size_t gene_contig_name_end = gene_header.find(':', gene_contig_name_beg);
		gene_contig_name.assign(gene_header, gene_contig_name_beg,  gene_contig_name_end-gene_contig_name_beg);

		return true;
	}

	return false;
};
