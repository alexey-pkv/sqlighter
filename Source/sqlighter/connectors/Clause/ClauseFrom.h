#ifndef SQLIGHTER_CLAUSEFROM_H
#define SQLIGHTER_CLAUSEFROM_H


#include <utility>

#include "Clause.h"
#include "ClauseTable.h"


namespace sqlighter
{
	template<class self>
	class ClauseFrom
	{
	private:
		ClauseTable m_from {};
	
	
	private:
		inline self& get_self() { return *static_cast<self*>(this); }
	
		
	protected:
		void append_from(std::ostream& ss) const
		{
			if (m_from.has_table())
			{
				ss << " FROM ";
				m_from.append_to(ss);
			}
		}
		
	
	public:
		self& from(std::string_view table, std::string_view as = {})
		{
			m_from.table(table, as);
			return get_self();
		}
		
		self& from_scheme(std::string_view scheme, std::string_view table, std::string_view as = {})
		{
			m_from.table_scheme(scheme, table, as);
			return get_self();
		}
	};
}


#endif
