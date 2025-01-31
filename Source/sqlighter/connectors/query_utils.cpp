#include "query_utils.h"


#include <sstream>


using namespace sqlighter;


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
