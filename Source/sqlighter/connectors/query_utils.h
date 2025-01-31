#pragma once


#include <string>
#include <sstream>


namespace sqlighter
{
	enum class OrderBy; 
	
	
	void element_name(std::string_view fullName, std::string& scheme, std::string& name);
	
	std::string create_questions(int count);
	
	
	struct col_as
	{
	public:
		std::string_view column;
		std::string_view as;
		
		
	public:
		constexpr col_as(std::string_view c, std::string_view a) : column(c), as(a) {};
	};
	
	
	inline std::ostream& operator<<(std::ostream& s, const sqlighter::col_as& c)
	{
		s << c.column << " as " << c.as;
		return s;
	}
}