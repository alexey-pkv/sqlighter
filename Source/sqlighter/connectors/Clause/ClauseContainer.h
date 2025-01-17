#pragma once


#include "Clause.h"


namespace sqlighter
{
	class ClauseContainer
	{
	private:
		Clause m_clause;
		
		
	protected:
		Clause& clause() { return m_clause; }
		const Clause& clause() const { return m_clause; }
		
		
	public:
		ClauseContainer(const char* c);
		
		
	public:
		void append(std::string_view exp, const std::vector<BindValue>& bind);
		
	
	public:
		inline void append_to(std::ostringstream& to) const { m_clause.append_to(to); }
		inline void append_to(std::vector<BindValue>& to) const { m_clause.append_to(to); }
		
		[[nodiscard]] inline bool empty_clause() const noexcept { return m_clause.empty_clause(); }
		[[nodiscard]] inline size_t binds_size() const noexcept { return m_clause.binds_size(); }
	};
}