/*
 * (C) Copyright 2015 Enterome
 */

#include <iostream>
#include "parameters.hh"
#include "time_profiler.hh"
#include "gene_to_cluster_indexer.hh"
#include "genes_connections_counter.hh"
#include "genes_connections_printer.hh"

int main(int argc, char* argv[])
{
	try
	{
		const Parameters& pars = Parameters::parse_cmd_line(argc, argv);
		std::cout << pars << std::endl;

		TimeProfiler time_profiler;
		time_profiler.start_new_timer("Total");

		std::cout << "Indexing genes..." << std::endl;
		time_profiler.start_new_timer("Indexing genes");
		const boost::unordered_map<std::string,uint32_t>& gene_to_cluster_map = GeneToClusterIndexer(pars.clusters_file).make_index();
		time_profiler.stop_last_timer();
		std::cout << "Done. " << gene_to_cluster_map.size() << " genes indexed\n" << std::endl;

		std::cout << "Counting genes connections..." << std::endl;
		time_profiler.start_new_timer("Count genes connections");
		GenesConnectionsCounter connections_counter(gene_to_cluster_map, pars.fasta_file);
		const boost::unordered_map<std::pair<uint32_t, uint32_t>,uint32_t>& genes_connections = connections_counter.count_connections();
		time_profiler.stop_last_timer();
		std::cout << "Done. " << genes_connections.size() << " pairs of connected genes found.\n" << std::endl;

		std::cout << "Printing genes connections..." << std::endl;
		time_profiler.start_new_timer("Printing genes connections");
		GenesConnectionsPrinter genes_connections_printer(genes_connections, pars.output_file, pars.min_num_connections);
		size_t num_connections_printed = genes_connections_printer.print();
		time_profiler.stop_last_timer();
		std::cout << "Done. " <<  num_connections_printed << " pairs of genes written after filtering.\n" << std::endl;

		time_profiler.stop_last_timer();
		std::cout << time_profiler;

		std::exit(0);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::exit(-1);
	}

}
