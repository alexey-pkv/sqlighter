#include "ClauseLimit.h"


using namespace sqlighter;


void ClauseLimit::limit_by(int count)
{
	m_hasLimit = true;
	
	m_limitCount = count;
	m_limitOffset = 0;
}

void ClauseLimit::limit(int offset, int count)
{
	m_hasLimit = true;
	
	m_limitCount = count;
	m_limitOffset = offset;
}

void ClauseLimit::page(int page, int page_size)
{
	m_hasLimit = true;
	
	m_limitCount = page_size;
	m_limitOffset = page * page_size;
}


void ClauseLimit::append_to(std::ostream& to) const
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


std::ostream& sqlighter::operator<<(std::ostream& ss, const ClauseLimit& ct)
{
	ct.append_to(ss);
	return ss;
}
