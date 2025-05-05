#include "StmtRef.h"


using namespace std;
using namespace sqlighter;


StmtRef::StmtRef(sqlite3_stmt* value)
{
	set(value);
}


sqlite3_stmt* StmtRef::get()
{
	if (m_unique)
		return m_unique.get();
	
	return m_weak.lock().get();
}

const sqlite3_stmt* StmtRef::get() const noexcept
{
	if (m_unique)
		return m_unique.get();
	
	return m_weak.lock().get();
}

void StmtRef::set(sqlite3_stmt* value)
{
	clear();
	m_unique = unique_stmt_ptr(value);
}
	
void StmtRef::set(std::shared_ptr<sqlite3_stmt> value)
{
	clear();
	
	if (value)
	{
		m_weak = value;
	}
}

void StmtRef::clear()
{
	m_unique = nullptr;
	m_weak.reset();
}

StmtRef::operator bool() const
{
	return m_unique || !m_weak.expired();
}
