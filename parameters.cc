/*
 * (C) Copyright 2015 Enterome
 */

#include "parameters.hh"
#include <iostream>
#include <fstream>
#include <exception>
#include <algorithm>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

Parameters Parameters::parse_cmd_line(int argc, char* argv[])
{
	Parameters pars;

	// Create options decription
	po::options_description opts_desc("");

	opts_desc.add_options()
		("clusters-file,c", po::value<std::string>(&pars.clusters_file), "")
		("fasta-file,f", po::value<std::string>(&pars.fasta_file), "")
		("output-file,o", po::value<std::string>(&pars.output_file), "")
		("min-num-connections", po::value<size_t>(&pars.min_num_connections)->default_value(3), "")
		("help,h", "display this help and exit.")
		;

	// Retrieve and parse command line Parameters
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, opts_desc), vm);

	// Print help
	if (argc == 1 || vm.count("help"))
	{
		std::cout << opts_desc << std::endl;
		std::exit(0);
	}

	po::notify(vm);

	check_file_is_readable(pars.clusters_file);
	check_file_is_readable(pars.fasta_file);
	check_file_is_writable(pars.output_file);

	return pars;
}

void Parameters::check_file_is_readable(const std::string& filepath)
{
	std::ifstream ifs;
	ifs.open(filepath.c_str());

	if (ifs.good())
	{
		ifs.close();
	}
	else
	{
		throw (std::invalid_argument("error: " + filepath +
					" cannot be opened. Check that the path is valid and that you have read permissions."));
	}
}

void Parameters::check_file_is_writable(const std::string& filepath)
{
	std::ofstream ofs;
	ofs.open(filepath.c_str());

	if (ofs.good())
	{
		ofs.close();
	}
	else
	{
		throw (std::invalid_argument("error: " + filepath +
					" cannot be created. Check that the path is valid and that you have write permissions."));
	}
}

std::ostream& operator<<(std::ostream& os, const Parameters& pars)
{
	os << "---------------------\n";
	os << "Parameters summary:\n";
	os << "--clusters-file = " << pars.clusters_file << '\n';
	os << "--fasta-file = " << pars.fasta_file << '\n';
	os << "--output-file = " << pars.output_file << '\n';
	os << "--min-num-connections = " << pars.min_num_connections << '\n';

	return os;
}
