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
	{
		throw SQLighterException(SQLIGHTER_ERR_UNEXPECTED, "db must not be null");
	}
}

Connection::Connection(const std::filesystem::path& db_path) :
	m_db(std::make_shared<DB>(db_path.c_str()))
{
	
}


Stmt Connection::execute(std::string_view query, const std::vector<BindValue>& values)
{
	Stmt stmt { m_db, query };
	
	if (query.empty())
	{
		throw SQLighterException(SQLIGHTER_ERR_EMPTY_QUERY);
	}
	
	m_db->open();
	
	auto res = sqlite3_prepare_v2(
		m_db->db(),
		query.data(),
		-1, 
		stmt.stmt_p(), 
		nullptr);
	
	if (res != SQLITE_OK)
	{
		auto e = SQLighterException(SQLIGHTER_ERR_PREPARE)
			.sqlite_error(res)
			.query(query)
			.db_msg(m_db);
		
		throw e;
	}
	
	for (auto i = 0; i < values.size(); i++)
	{
		values[i].bind(stmt.stmt(), i + 1);
	}
	
	stmt.step();
	
	return stmt;
}
