#include "StmtRef.h"


using namespace std;
using namespace sqlighter;


StmtRef::StmtRef(weak_stmt_ptr weak) : 
	m_weak(weak)
{
	
}

StmtRef::StmtRef(shared_stmt_ptr m_stmt) : 
	m_stmt(m_stmt)
{
	
}

StmtRef::StmtRef(sqlite3_stmt* value) : 
	m_stmt(value)
{
	
}


sqlite3_stmt* StmtRef::get()
{
	if (m_stmt)
		return m_stmt.get();
	
	return m_weak.lock().get();
}

const sqlite3_stmt* StmtRef::get() const noexcept
{
	if (m_stmt)
		return m_stmt.get();
	
	return m_weak.lock().get();
}

void StmtRef::clear()
{
	m_stmt = nullptr;
	m_weak.reset();
}

StmtRef::operator bool() const
{
	return m_stmt || !m_weak.expired();
}
