#pragma once


#include "Clause.h"
#include "consts/OrderBy.h"
#include "consts/const_cast.h"


namespace sqlighter
{
	template<class self>
	class ClauseOrderBy
	{
	private:
		Clause m_orderBy { ", " };
		
		
	private:
		inline self& get_self() { return *static_cast<self*>( this ); }
		
		
	protected:
		void append_order_by(std::ostream& to) const
		{
			if (m_orderBy.is_empty_clause())
				return;
			
			to << " ORDER BY " << m_orderBy;
		}
		
		
	protected:
		inline Clause& order_by_clause() { return m_orderBy; }
		inline const Clause& order_by_clause() const { return m_orderBy; }
		
		
	public:
		inline self& order_by(std::string_view exp, const std::vector<BindValue>& bind)
		{
			m_orderBy.append(exp, bind);
			return get_self();
		}
		
		self& order_by_field(std::string_view by, OrderBy order)
		{
			m_orderBy.next_section() << by;
			
			if (order == OrderBy::DESC)
			{
				m_orderBy.direct() << ' ' << order;
			}
			
			return get_self();
		}
		
		inline self& order_by_field(std::string_view by) { return order_by_field(by, OrderBy::ASC); }
		inline self& order_by_field_asc(std::string_view by) { return order_by_field(by, OrderBy::ASC); }
		inline self& order_by_field_desc(std::string_view by) { return order_by_field(by, OrderBy::DESC); }
		
		inline self& order_by(std::string_view exp)												{ return order_by(exp, {}); };
		inline self& order_by(std::string_view exp, BindValue value)							{ return order_by(exp, { value }); };
		inline self& order_by(std::string_view exp, std::initializer_list<BindValue> values)	{ return order_by(exp, { values }); };
	};
}