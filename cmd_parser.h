#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "custom_exceptions.h"
#include "fileconverter_to_hash.h"

using namespace boost::program_options;
using namespace boost::filesystem;

class cdm_parser
{
public:
	cdm_parser( int argc, const char *argv[] )
		:m_block_size(0)
	{

		options_description desc{"Options"};
		desc.add_options()
				("help,h", "Help screen")
				("in", value<std::string>()->default_value("big_file.txt"), "In")
				("out", value<std::string>()->default_value("file_hash.txt"), "Out")
				("size", value<uint32_t>()->default_value(1000), "Size");

		variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);

		if (vm.count("help"))
		{
			std::cout << desc << '\n';
		}
		if (vm.count("in"))
		{
			m_input_file_path = vm["in"].as<std::string>();
			path p (m_input_file_path);
			if (exists(p))
			{
				if (is_regular_file(p))
				{
					if (!file_size(p))
					{
						throw parser_exception( 1002, "Parser", "File size is null!" );
					}
				}
				else
				{
					throw parser_exception( 1002, "Parser", "Input file does not exist" );
				}
			}
		}
		if (vm.count("out"))
		{
			m_output_file_path = vm["out"].as<std::string>();
			path p (m_output_file_path);
			if (exists(p))
			{
				if (is_regular_file(p))
				{
					std::cout << "output file is already exsit - will be replace with new one" << std::endl;
				}
			}
		}
		if (vm.count("size"))
		{
			m_block_size = vm["size"].as<uint32_t>();
		}
	}

	std::string get_input_file_path() const noexcept
	{
		return m_input_file_path;
	}

	std::string get_ouput_file_path() const noexcept
	{
		return m_output_file_path;
	}

	int get_block_size() const noexcept
	{
		return m_block_size;
	}

	~cdm_parser()
	{
	}
private:
	std::string m_input_file_path;
	std::string m_output_file_path;
	uint32_t m_block_size;
};

#endif // CMD_PARSER_H
