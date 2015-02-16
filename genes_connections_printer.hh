#ifndef GENES_CONNECTIONS_PRINTER_HH
#define GENES_CONNECTIONS_PRINTER_HH

/*
 * (C) Copyright 2015 Enterome
 */

#include <boost/unordered_map.hpp>
#include <string>

class GenesConnectionsPrinter
{
	public:
		GenesConnectionsPrinter(const boost::unordered_map<std::pair<uint32_t, uint32_t>,uint32_t>& genes_connections, const std::string& output_file, size_t min_num_connections);
		size_t print();

		const boost::unordered_map<std::pair<uint32_t, uint32_t>,uint32_t>& genes_connections_;
		const std::string output_file_;
		const size_t min_num_connections_;
};

inline GenesConnectionsPrinter::GenesConnectionsPrinter(const boost::unordered_map<std::pair<uint32_t, uint32_t>,uint32_t>& genes_connections, const std::string& output_file, size_t min_num_connections)
	: genes_connections_(genes_connections), output_file_(output_file), min_num_connections_(min_num_connections)
{}

#endif // GENES_CONNECTIONS_PRINTER_HH
