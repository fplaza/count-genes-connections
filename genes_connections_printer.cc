/*
 * (C) Copyright 2015 Enterome
 */

#include "genes_connections_printer.hh"
#include <fstream>

size_t GenesConnectionsPrinter::print()
{
	std::ofstream ofs;
	ofs.open(output_file_.c_str());

	size_t num_connections_printed = 0;
	boost::unordered_map<std::pair<uint32_t, uint32_t>,uint32_t>::const_iterator genes_connections_it;
	for (genes_connections_it = genes_connections_.begin(); genes_connections_it != genes_connections_.end(); ++genes_connections_it)
	{
		const std::pair<uint32_t, uint32_t>& genes_pair = genes_connections_it->first;
		const uint32_t num_connections = genes_connections_it->second;

		if (num_connections >= min_num_connections_)
		{
			ofs << genes_pair.first << '\t' << genes_pair.second << '\t' << num_connections << '\n';
			num_connections_printed++;
		}
	}

	ofs.close();

	return num_connections_printed;
}
