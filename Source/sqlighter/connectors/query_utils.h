#pragma once


#include <string>


namespace sqlighter
{
	enum class OrderBy; 
	
	
	std::string wrap_element(std::string_view element, std::string_view as);
	std::string wrap_element(std::string_view);
	
	void wrap_element(std::ostringstream& ss, std::string_view element, std::string_view as);
	void wrap_element(std::ostringstream& ss, std::string_view element);
	
	
	struct col
	{
	public:
		std::string_view column;
		
		
	public:
		constexpr explicit col(std::string_view c) : column(c) {};
	};
	
	struct col_as
	{
	public:
		std::string_view column;
		std::string_view as;
		
		
	public:
		constexpr col_as(std::string_view c, std::string_view a) : column(c), as(a) {};
	};
	
	
	inline std::ostringstream& operator<<(std::ostringstream& s, const sqlighter::col& c)
	{
		sqlighter::wrap_element(s, c.column);
		return s;
	}
	
	inline std::ostringstream& operator<<(std::ostringstream& s, const sqlighter::col_as& c)
	{
		sqlighter::wrap_element(s, c.column, c.as);
		return s;
	}
}