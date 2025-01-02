#include "query_utils.h"


#include <sstream>


using namespace sqlighter;


col::col(std::string_view c) : column(c) 
{	
}


col_as::col_as(std::string_view c, std::string_view as) :
	column(c),
	as(as)
{	
}


std::string sqlighter::wrap_element(std::string_view element, std::string_view as)
{
	std::stringstream ss;
	wrap_element(ss, element, as);
	return ss.str();
}

void sqlighter::wrap_element(std::stringstream& ss, std::string_view element, std::string_view as)
{
	ss << '`' << element << "` as `" << as << '`';
}


std::string sqlighter::wrap_element(std::string_view element)
{
	std::stringstream ss;
	wrap_element(ss, element);
	return ss.str();
}

void sqlighter::wrap_element(std::stringstream& ss, std::string_view element)
{
	ss << '`' << element << '`';
}
