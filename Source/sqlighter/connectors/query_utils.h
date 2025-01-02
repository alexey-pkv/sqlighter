#pragma once


#include <string>


namespace sqlighter
{
	enum class OrderBy; 
	
	
	std::string wrap_element(std::string_view element, std::string_view as);
	std::string wrap_element(std::string_view);
	
	void wrap_element(std::stringstream& ss, std::string_view element, std::string_view as);
	void wrap_element(std::stringstream& ss, std::string_view element);
	
	
	struct col
	{
	public:
		std::string_view column;
		
		
	public:
		explicit col(std::string_view);
	};
	
	struct col_as
	{
	public:
		std::string_view column;
		std::string_view as;
		
		
	public:
		col_as(std::string_view c, std::string_view a);
	};
}


inline std::stringstream& operator<<(std::stringstream& s, const sqlighter::col& c)
{
	sqlighter::wrap_element(s, c.column);
	return s;
}

inline std::stringstream& operator<<(std::stringstream& s, const sqlighter::col_as& c)
{
	sqlighter::wrap_element(s, c.column, c.as);
	return s;
}