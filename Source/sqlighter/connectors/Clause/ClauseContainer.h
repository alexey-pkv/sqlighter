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
		inline void append_binds(std::vector<BindValue>& to) const { m_clause.append_to(to); }
		[[nodiscard]] inline size_t binds_size() const noexcept { return m_clause.binds_size(); }
	
		
	public:
		virtual void append_to(std::ostream& to) const = 0;
	};
	
	
	std::ostream& operator<<(std::ostream& ss, const ClauseContainer& ct);
}