#include "sqlighter.h"


#include "core/DB.h"
#include "connectors/query_utils.h"


using namespace sqlighter;


SQLighter::SQLighter(const std::filesystem::path& db_path) :
	m_path(db_path)
{
	
}

SQLighter::SQLighter(const char* db_path) :
	m_path(db_path)
{
	
}

SQLighter::SQLighter(const std::shared_ptr<DB>& db) :
	m_path(db->path()),
	m_db(db),
	m_connection(std::make_shared<Connection>(db))
{
	
}


const std::shared_ptr<Connection>& SQLighter::connection()
{
	open();
	
	return m_connection;
}


CMDSelect SQLighter::select()
{
	return CMDSelect { connection() };
}

CMDDirect SQLighter::direct()
{
	return CMDDirect { connection() };
}

CMDCreateTable SQLighter::create()
{
	return CMDCreateTable { connection() };
}

CMDDrop SQLighter::drop()
{
	return CMDDrop { connection() };
}

CMDInsert SQLighter::insert()
{
	return CMDInsert { connection() };
}

CMDUpdate SQLighter::update()
{
	return CMDUpdate { connection() };
}

CMDDelete SQLighter::del()
{
	return CMDDelete { connection() };
}

Stmt SQLighter::execute(std::string_view query)
{
	return direct()
		.append(query)
		.execute();
}

Stmt SQLighter::execute(std::string_view query, const BindValue& value)
{
	return direct()
		.append(query, { value })
		.execute();
}

Stmt SQLighter::execute(std::string_view query, std::initializer_list<BindValue> values)
{
	return direct()
		.append(query, values)
		.execute();
}

int64_t SQLighter::count_rows(std::string_view table)
{
	return select().from(table).query_count();
}

std::vector<std::vector<ScalarValue>> SQLighter::query_all(std::string_view table, int failsafeLimit)
{
	return select().from(table).query_all(failsafeLimit);
}

const std::string& SQLighter::path() const
{
	return m_path;
}

void SQLighter::close()
{
	if (m_db != nullptr)
	{
		m_db->close();
	}
	
	m_db = nullptr;
	m_connection = nullptr;
}

void SQLighter::open()
{
	if (is_open())
		return;
	
	m_db = std::make_shared<DB>(m_path);
	m_connection = std::make_shared<Connection>(m_db);
	
	m_db->open();
}

bool SQLighter::is_open() const noexcept
{
	return m_db && m_db->is_open();
}

void SQLighter::reindex(std::string_view element)
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

void SQLighter::reindex(std::string_view scheme, std::string_view element)
{
	auto d = direct();
	
	d << "REINDEX `" << scheme << "`.`" << element << "`";
	
	d.execute();
}

std::string SQLighter::sqlite_version()
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
