#ifndef CUSTOM_EXEPTIONS_H
#define CUSTOM_EXEPTIONS_H

#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <iomanip>
#include <algorithm>

class parser_exception : public std::exception
{

public:
	parser_exception();
	explicit parser_exception(const std::string& message);
	parser_exception(const std::string& source, const std::string& message);
	parser_exception(int code, const std::string& source, const std::string& message);
	const char *what() const throw();

private:
	// helper function
	static std::string make_message(int code, const std::string& source, const std::string& message);
	std::string message;
};

#endif // CUSTOM_EXEPTIONS_H
