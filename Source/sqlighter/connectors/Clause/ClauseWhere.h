#ifndef SQLIGHTER_CLAUSEWHERE_H
#define SQLIGHTER_CLAUSEWHERE_H


#include <utility>

#include "Clause.h"


namespace sqlighter
{
	template<class self>
	class ClauseWhere
	{
	private:
		Clause m_where { " AND " };
		
		
	private:
		inline self& get_self() { return *static_cast<self*>(this); }
		
		
	protected:
		Clause& where_clause() { return m_where; }
		const Clause& where_clause() const { return m_where; }
		
		
	public:
		self& where(std::string_view exp, const std::vector<BindValue>& values)
		{
			m_where.append(exp, values);
			return get_self();
		}
		
		self& where(std::string_view exp)											{ return where(exp, std::vector<BindValue> {}); };
		self& where(std::string_view exp, BindValue value)							{ return where(exp, std::vector<BindValue> { std::move(value) }); };
		self& where(std::string_view exp, std::initializer_list<BindValue> values)	{ return where(exp, std::vector<BindValue> { values }); };
		
		self& where_null(std::string_view column)
		{
			m_where.next_section() << column << " IS NULL";
			return get_self();
		}
		
		self& where_not_null(std::string_view column)
		{
			m_where.next_section() << column << " IS NOT NULL";
			return get_self();
		}
		
		self& by_field(std::string_view column, const BindValue& value)
		{
			m_where.next_section() << column << " = ?";
			m_where.append_bind(value);
			return get_self();
		}
	};
}


#endif