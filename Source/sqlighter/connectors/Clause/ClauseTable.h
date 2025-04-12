#ifndef SQLIGHTER_CONNECTORS_CLAUSE_CLAUSETABLE_H
#define SQLIGHTER_CONNECTORS_CLAUSE_CLAUSETABLE_H


#include <string>
#include <sstream>


namespace sqlighter
{
	class ClauseTable
	{
	private:
		std::string	m_table		{};
		std::string	m_scheme	{};
		std::string	m_as		{};
		
		
	public:
		void as(std::string_view alias);
		void table(std::string_view, std::string_view as = {});
		void table_scheme(std::string_view scheme, std::string_view table, std::string_view as = {});
		
		
	public:
		const std::string& scheme() const noexcept { return m_scheme; }
		const std::string& table() const noexcept { return m_table; }
		const std::string& alias() const noexcept { return m_as; }
		bool has_scheme() const noexcept { return !m_scheme.empty(); }
		bool has_table() const noexcept { return !m_table.empty(); }
		bool has_alias() const noexcept { return !m_as.empty(); }
		
		
	public:
		void append_to(std::ostream& to) const;
	};
	
	
	std::ostream& operator<<(std::ostream& ss, const ClauseTable& ct);
}


#endif
