#include "Connection.h"


#include "core/DB.h"
#include "core/Stmt.h"
#include "core/BindValue.h"
#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


Connection::Connection(const std::shared_ptr<DB>& db) : 
	m_db(db)
{
	if (db == nullptr)
		throw SQLighterException();
}


Stmt Connection::execute(std::string_view query, const std::vector<BindValue>& values)
{
	Stmt stmt {};
	
	m_db->open();
	
	auto res = sqlite3_prepare_v2(
		m_db->db(),
		query.data(),
		-1, 
		stmt.stmt_p(), 
		nullptr);
	
	if (res != SQLITE_OK)
	{
		throw SQLighterException();
	}
	
	for (auto i = 0; i < values.size(); i++)
	{
		values[i].bind(stmt.stmt(), i);
	}
	
	stmt.step();
	
	return stmt;
}
