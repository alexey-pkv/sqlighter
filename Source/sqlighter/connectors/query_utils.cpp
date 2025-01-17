#include "query_utils.h"


#include <sstream>


using namespace sqlighter;


std::ostream& sqlighter::wrap_element(std::ostream& ss, std::string_view element, std::string_view as)
{
	ss << '`' << element << "` as `" << as << '`';
	return ss;
}

std::ostream& sqlighter::wrap_element(std::ostream& ss, std::string_view element)
{
	ss << '`' << element << '`';
	return ss;
}


void sqlighter::element_name(std::string_view fullName, std::string& scheme, std::string& name)
{
	auto at = fullName.find('.');
	
	if (at != std::string::npos)
	{
		scheme = fullName.substr(0, at);
		name = fullName.substr(at + 1);
	}
	else
	{
		name = fullName;
	}
}

std::string sqlighter::create_questions(int count)
{
	if (count <= 0)
		return { "" };
	else if (count == 1)
		return { "?" };
	
	std::ostringstream ss;
	
	for (int i = 0; i < count - 1; i++)
	{
		ss << "?, ";
	}
	
	ss << "?";
	
	return ss.str();
}
