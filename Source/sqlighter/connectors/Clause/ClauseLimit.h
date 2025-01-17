#pragma once


#include <sstream>


namespace sqlighter
{
	class ClauseLimit
	{
	private:
		bool m_hasLimit	= false;
		
		int m_limitOffset = 0;
		int m_limitCount = 0;
		
		
	public:
		void limit_by(int count);
		void limit(int offset, int count);
		void page(int page, int page_size);
		
		
	public:
		[[nodiscard]] bool is_empty() const { return !m_hasLimit; }
		
		
	public:
		void append_to(std::ostream& to) const;
	};
	
	
	std::ostream& operator<<(std::ostream& ss, const ClauseLimit& ct);
}

#define SQLIGHTER_LIMIT_CLAUSE(data_member, self)						\
	public:																\
		inline self& limit_by(int count)								\
		{ data_member.limit_by(count); return *this; }					\
																		\
		inline self& limit(int offset, int count) 						\
		{ data_member.limit(offset, count); return *this; }				\
        																\
		inline self& page(int page, int page_size)         				\
		{ data_member.page(page, page_size); return *this; }

		