#ifndef PARAMETERS_HH
#define PARAMETERS_HH

/*
 * (C) Copyright 2015 Enterome
 */

#include <vector>
#include <string>

struct Parameters
{
	std::string clusters_file;
	std::string fasta_file;
	std::string output_file;
	size_t min_num_connections;

	static Parameters parse_cmd_line(int argc, char* argv[]);
	static void check_file_is_readable(const std::string& filepath);
	static void check_file_is_writable(const std::string& filepath);
};

std::ostream& operator<<(std::ostream& os, const Parameters& pars);

#endif // PARAMETERS_HH
