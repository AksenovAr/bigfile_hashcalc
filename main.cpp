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
		handler_t f = [](std::string& s){ std::cout << s << std::endl;};
		cdm_parser parser(argc, argv);
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
