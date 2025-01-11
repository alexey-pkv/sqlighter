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
		throw SQLighterException(SQLIGHTER_ERR_FAILED_TO_CLOSE_DB, res);
	}
}

void DB::open()
{
	if (m_db)
		return;
	
	if (m_path.empty())
		throw SQLighterException(SQLIGHTER_ERR_FAILED_TO_OPEN_DB, "Path for database not provided");
	
	auto res = sqlite3_open_v2(
		m_path.c_str(),
		&m_db,
		SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
		nullptr);
	
	if (res != SQLITE_OK)
	{
		if (m_db)
		{
			sqlite3_close_v2(m_db);
			m_db = nullptr;
		}
		
		throw SQLighterException(SQLIGHTER_ERR_FAILED_TO_OPEN_DB, res);
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

const char* DB::error_message() const
{
	if (!m_db)
		return "";
	
	return sqlite3_errmsg(m_db);
}
