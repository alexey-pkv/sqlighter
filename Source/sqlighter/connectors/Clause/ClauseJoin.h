#ifndef SQLIGHTER_CLAUSEJOIN_H
#define SQLIGHTER_CLAUSEJOIN_H


#include <utility>

#include "Clause.h"


namespace sqlighter
{
	template<class self>
	class ClauseJoin
	{
	private:
		bool					m_isEmpty	= true;
		std::ostringstream		m_stream;
		std::vector<BindValue>	m_binds;
		
		
	private:
		inline self& get_self() { return *static_cast<self*>(this); }
		
		
	protected:
		inline size_t join_bind_size() const { return m_binds.size(); }
		
		void append_join(std::ostream& ss) const
		{
			if (!m_isEmpty)
			{
				ss << m_stream.str();
			}
		}
		
		void append_join_binds(std::vector<BindValue>& to) const
		{
			if (!m_binds.empty())
			{
				to.insert(to.end(), m_binds.begin(), m_binds.end());
			}
		}
		
		
	public:
		ClauseJoin() = default;
		ClauseJoin(ClauseJoin&&) noexcept = default;
		ClauseJoin& operator=(ClauseJoin&&) noexcept = default;
		
		ClauseJoin(const ClauseJoin& c)
		{
			*this = c;
		}
		
		ClauseJoin& operator=(const ClauseJoin& c)
		{
			if (&c == this)
				return *this;
			
			m_stream.str("");
			m_stream.clear();
			
			m_stream << c.m_stream.str();
			m_binds	= c.m_binds;
			m_isEmpty = c.m_isEmpty;
			
			return *this;
		}
		
		
	public:
		self& join_statement(
			std::string_view join_type,
			std::string_view scheme,
			std::string_view what,
			std::string_view as,
			std::string_view condition,
			const std::vector<BindValue>& binds)
		{
			m_isEmpty = false;
			
			m_stream << " " << join_type << " ";
			
			if (!scheme.empty())
			{
				m_stream << scheme << ".";
			}
			
			m_stream << what << " AS " << as;
			
			if (!condition.empty())
			{
				m_stream << " ON " << condition;
			}
			
			if (!binds.empty())
			{
				m_binds.insert(m_binds.end(), binds.begin(), binds.end());
			}
			
			return get_self();
		}
		
		self& join_exp(std::string_view exp, const std::vector<BindValue>& binds)
		{
			if (!exp.empty())
			{
				m_isEmpty = false;
				m_stream << " " << exp;
			}
			
			if (!binds.empty())
			{
				m_binds.insert(m_binds.end(), binds.begin(), binds.end());
			}
			
			return get_self();
		}
		
		self& join(
			std::string_view what,
			std::string_view as,
			std::string_view condition,
			const std::vector<BindValue>& binds)
		{
			return join_statement("JOIN", "", what, as, condition, binds);
		}
		
		self& left_join(
			std::string_view what,
			std::string_view as,
			std::string_view condition,
			const std::vector<BindValue>& binds)
		{
			return join_statement("LEFT JOIN", "", what, as, condition, binds);
		}
		
		self& right_join(
			std::string_view what,
			std::string_view as,
			std::string_view condition,
			const std::vector<BindValue>& binds)
		{
			return join_statement("RIGHT JOIN", "", what, as, condition, binds);
		}
	};
}


#endif
