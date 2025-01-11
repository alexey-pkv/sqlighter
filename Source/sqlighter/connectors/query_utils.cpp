#include "query_utils.h"


#include <sstream>


using namespace sqlighter;


std::string sqlighter::wrap_element(std::string_view element, std::string_view as)
{
	std::ostringstream ss;
	wrap_element(ss, element, as);
	return ss.str();
}

void sqlighter::wrap_element(std::ostringstream& ss, std::string_view element, std::string_view as)
{
	ss << '`' << element << "` as `" << as << '`';
}



std::string sqlighter::wrap_element(std::string_view element)
{
	std::ostringstream ss;
	wrap_element(ss, element);
	return ss.str();
}

void sqlighter::wrap_element(std::ostringstream& ss, std::string_view element)
{
	ss << '`' << element << '`';
}

