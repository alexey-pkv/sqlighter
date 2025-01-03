#include "DB.h"
#include "exceptions/sqlighter_exceptions.h"


#include <sqlite3.h>


using namespace sqlighter;


DB::DB(std::string_view path) : 
	m_path(path)
{
	
}

DB::~DB()
{
	close();
}


void DB::close()
{
	if (!m_db)
		return;
	
	auto res = sqlite3_close(m_db);
	m_db = nullptr;
	
	if (res != SQLITE_OK)
	{
		throw SQLighterException();
	}
}

void DB::open()
{
	if (m_db)
		return;
	
	if (m_path.empty())
		throw SQLighterException();
	
	auto res = sqlite3_open_v2(
		m_path.c_str(),
		&m_db,
		SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
		nullptr);
	
	if (res != SQLITE_OK)
	{
		m_db = nullptr;
		throw SQLighterException();
	}
}

void DB::open(std::string_view path)
{
	if (m_db && m_path == path)
		return;
	
	close();
	m_path = path;
	
	open();
}
