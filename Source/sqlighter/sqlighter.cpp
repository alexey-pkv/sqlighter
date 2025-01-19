#include "sqlighter.h"


#include "core/DB.h"
#include "connectors/query_utils.h"


using namespace sqlighter;


SQLighter::SQLighter(const std::filesystem::path& db_path) :
	m_db(std::make_shared<DB>(db_path.c_str())),
	m_connection(std::make_shared<Connection>(m_db))
{
	
}

SQLighter::SQLighter(const char* db_path) :
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

CMDDrop SQLighter::drop() const
{
	return CMDDrop { m_connection };
}

CMDInsert SQLighter::insert() const
{
	return CMDInsert { m_connection };
}

CMDUpdate SQLighter::update() const
{
	return CMDUpdate { m_connection };
}

CMDDelete SQLighter::del() const
{
	return CMDDelete { m_connection };
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

Stmt SQLighter::execute(std::string_view query, std::initializer_list<BindValue> values) const
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


void SQLighter::reindex(std::string_view element) const
{
	auto d = direct();
	
	std::string scheme	{};
	std::string name	{};
	
	element_name(element, scheme, name);
	
	if (scheme.empty())
	{
		d << "REINDEX `" << name << "`";
	}
	else
	{
		d << "REINDEX `" << scheme << "`.`" << name << "`";
	}
	
	d.execute();
}

void SQLighter::reindex(std::string_view scheme, std::string_view element) const
{
	auto d = direct();
	
	d << "REINDEX `" << scheme << "`.`" << element << "`";
	
	d.execute();
}

std::string SQLighter::sqlite_version() const
{
	return select().column_exp("sqlite_version()").query_str();
}


std::string sqlighter::sqlite_lib_version()
{
	return sqlite3_libversion();
}

std::string sqlighter::sqlighter_lib_version()
{
	return SQLIGHTER_VERSION;
}
