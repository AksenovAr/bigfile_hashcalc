#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <functional>
#include "custom_exceptions.h"
#include "fileconverter_to_hash.h"
#include "cmd_parser.h"

int main(int argc, const char *argv[])
{
	try
	{
		cdm_parser parser(argc, argv);
		handler_t f = [&](std::ostringstream& s)
		{
			std::ofstream out(parser.get_ouput_file_path(), std::ofstream::out | std::ofstream::binary);
			out << s.str();
			out.close();
		};

		std::shared_ptr<fileconverter_to_hash> ptr_obj = fileconverter_to_hash::getInstance();
		ptr_obj->add_handler(f);
		ptr_obj->doWork(parser);
	}
	catch (const error &ex)
	{
		std::cout << "Exception was catched : " << ex.what() << std::endl;
	}
	catch (const std::exception &ex)
	{
		std::cout << "Exception was catched : " << ex.what() << std::endl;
	}
	return 0;
}
