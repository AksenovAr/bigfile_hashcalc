#include "custom_exceptions.h"

parser_exception::parser_exception() :
parser_exception(0, "No source.", "No message.")
{

}

parser_exception::parser_exception(const std::string& message) :
parser_exception(0, "No source.", std::move(message))
{

}

parser_exception::parser_exception(const std::string& source, const std::string& message) :
parser_exception(0, std::move(source), std::move(message))
{

}

parser_exception::parser_exception(int code, const std::string& source, const std::string& message) :
message(make_message(code, source, message))
{}

const char *parser_exception::what() const throw()
{
	// message is a class member, not a temporary
	return message.c_str();
}

std::string parser_exception::make_message(int code, const std::string& source, const std::string& message)
{
	std::stringstream s;
	s << "MyException Data:" << std::endl;
	s << "Code    : " << code << std::endl;
	s << "Source  : " << source << std::endl;
	s << "Message : " << message << std::endl;

	// takes a copy, returns a copy - safe!
	return s.str();
}

//-------------------------------------------------------------------------------------------------------//


fileconverter_exception::fileconverter_exception() :
fileconverter_exception(0, "No source.", "No message.")
{

}

fileconverter_exception::fileconverter_exception(const std::string& message) :
fileconverter_exception(0, "No source.", std::move(message))
{

}

fileconverter_exception::fileconverter_exception(const std::string& source, const std::string& message) :
fileconverter_exception(0, std::move(source), std::move(message))
{

}

fileconverter_exception::fileconverter_exception(int code, const std::string& source, const std::string& message) :
message(make_message(code, source, message))
{}

const char *fileconverter_exception::what() const throw()
{
	// message is a class member, not a temporary
	return message.c_str();
}

std::string fileconverter_exception::make_message(int code, const std::string& source, const std::string& message)
{
	std::stringstream s;
	s << "MyException Data:" << std::endl;
	s << "Code    : " << code << std::endl;
	s << "Source  : " << source << std::endl;
	s << "Message : " << message << std::endl;

	// takes a copy, returns a copy - safe!
	return s.str();
}
