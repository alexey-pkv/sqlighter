#pragma once


#include <sstream>


namespace sqlighter
{
	template<class self>
	class ClauseLimit
	{
	private:
		bool m_hasLimit	= false;
		
		int m_limitOffset = 0;
		int m_limitCount = 0;
		
		
	private:
		inline self& get_self() { return *static_cast<self*>( this ); }
		
		
	protected:
		void append_limit(std::ostream& to) const
		{
			if (!m_hasLimit)
				return;
			
			to << " LIMIT ";
			
			if (m_limitOffset != 0)
			{
				to << m_limitOffset << ", ";
			}
			
			to << m_limitCount;
		}
		
		
	public:
		self& limit_by(int count)
		{
			m_hasLimit = true;
			
			m_limitCount = count;
			m_limitOffset = 0;
			
			return get_self();
		}
		
		self& limit(int offset, int count)
		{
			m_hasLimit = true;
			
			m_limitCount = count;
			m_limitOffset = offset;
			
			return get_self();
		}
		
		self& page(int page, int page_size)
		{
			m_hasLimit = true;
			
			m_limitCount = page_size;
			m_limitOffset = page * page_size;
			
			return get_self();
		}
	};
}