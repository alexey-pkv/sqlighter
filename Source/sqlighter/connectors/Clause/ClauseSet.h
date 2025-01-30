#ifndef SQLIGHTER_CLAUSESET_H
#define SQLIGHTER_CLAUSESET_H


#include "Clause.h"


namespace sqlighter
{
	template<class self>
	class ClauseSet
	{
	private:
		Clause m_set { ", " };
		
		
	private:
		inline self& get_self() { return *static_cast<self*>( this ); }
		
		
	protected:
		inline Clause& set_clause() { return m_set; }
		inline const Clause& set_clause() const { return m_set; }
		
		
	public:
		self& set_exp(std::string_view exp, const std::vector<BindValue>& values)
		{
			m_set.append(exp, values);
			return get_self();
		}
		
		self& set(std::string_view field, BindValue value)
		{
			m_set << col(field) << " = ?";
			m_set.append_bind(value);
			return get_self();
		}
		
		
		inline self& set_exp(std::string_view exp)											{ return set_exp(exp, {}); };
		inline self& set_exp(std::string_view exp, BindValue value)							{ return set_exp(exp, { value }); };
		inline self& set_exp(std::string_view exp, std::initializer_list<BindValue> values)	{ return set_exp(exp, { values }); };
	};
}

#endif