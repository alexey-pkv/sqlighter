#include "sqlighter.h"


#include "core/DB.h"
#include "connectors/CMDSelect.h"
#include "connectors/CMDDirect.h"


using namespace sqlighter;


SQLighter::SQLighter(const std::filesystem::path& db_path) :
	m_db(std::make_shared<DB>(db_path.c_str())),
	m_connection(std::make_shared<Connection>(m_db))
{
	
}

SQLighter::SQLighter(std::string_view db_path) :
	m_db(std::make_shared<DB>(db_path)),
	m_connection(std::make_shared<Connection>(m_db))
{
	
}

SQLighter::SQLighter(const std::shared_ptr<DB>& db) :
	m_db(db),
	m_connection(std::make_shared<Connection>(db))
{
	
}


CMDSelect SQLighter::select() const
{
	return CMDSelect { m_connection };
}

CMDDirect SQLighter::direct() const
{
	return CMDDirect { m_connection };
}

CMDCreateTable SQLighter::create() const
{
	return CMDCreateTable { m_connection };
}

Stmt SQLighter::execute(std::string_view query) const
{
	return direct()
		.append(query)
		.execute();
}

Stmt SQLighter::execute(std::string_view query, const BindValue& value) const
{
	return direct()
		.append(query, { value })
		.execute();
}

Stmt SQLighter::execute(std::string_view query, std::initializer_list<const BindValue> values) const
{
	return direct()
		.append(query, values)
		.execute();
}

int SQLighter::count_rows(std::string_view table) const
{
	return select().from(table).query_count();
}

std::vector<std::vector<ScalarValue>> SQLighter::query_all(std::string_view table, int failsafeLimit) const
{
	return select().from(table).query_all(failsafeLimit);
}

const std::string& SQLighter::path() const
{
	return m_db->path();
}
