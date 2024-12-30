#pragma once


#include <string>


namespace sqlighter
{
	enum class OrderBy; 
	
	
	std::string wrap_element(std::string_view element, std::string_view as);
	std::string wrap_element(std::string_view);
	
	void wrap_element(std::stringstream& ss, std::string_view element, std::string_view as);
	void wrap_element(std::stringstream& ss, std::string_view element);
}