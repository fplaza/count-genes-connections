#ifndef FASTA_READER_HH
#define	FASTA_READER_HH

/*
 * (C) Copyright 2015 Enterome
 */

#include <string>
#include <fstream>

class FastaReader
{
	public:
		FastaReader(const std::string& fasta_file);
		~FastaReader();
		bool next_record(std::string& record_header);

	private:
		static const char NEW_RECORD_TOKEN = '>';
		static const size_t LINE_INITIAL_SIZE = 1024;
		const std::string& fasta_file_;
		std::ifstream stream_;
		std::string line_;
};


inline FastaReader::FastaReader(const std::string& fasta_file)
	: fasta_file_(fasta_file), stream_(fasta_file_.c_str()), line_()
{
	line_.reserve(LINE_INITIAL_SIZE);
}

inline FastaReader::~FastaReader()
{
	stream_.close();
}


inline bool FastaReader::next_record(std::string& record_header)
{
	if (!stream_.good())
		return false;

	while(std::getline(stream_, line_))
	{
		// We just need the header of each record
		if (!line_.empty() && line_[0] == NEW_RECORD_TOKEN)
		{
			record_header.assign(line_, 1, std::string::npos);
			return true;
		}
	}

	return false;
}

#endif // FASTA_READER_HH
