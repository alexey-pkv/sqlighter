#pragma once


#include "connection/Connection.h"
#include "connectors/CMDSelect.h"
#include "connectors/CMDDirect.h"
#include "connectors/CMDCreateTable.h"


namespace sqlighter
{
	class SQLighter
	{
	private:
		std::shared_ptr<DB>			m_db;
		std::shared_ptr<Connection>	m_connection;
		
		
	public:
		~SQLighter() = default;
		SQLighter() = delete;
		
		SQLighter(SQLighter&&) = default;
		SQLighter(const SQLighter&) = default;
		SQLighter& operator=(SQLighter&&) = default;
		SQLighter& operator=(const SQLighter&) = default;
		
		explicit SQLighter(const std::filesystem::path& db_path);
		explicit SQLighter(std::string_view db_path);
		explicit SQLighter(const std::shared_ptr<DB>& db);
		
		
	public:
		CMDSelect select() const;
		CMDDirect direct() const;
		CMDCreateTable create() const;
		
		Stmt execute(std::string_view query) const;
		Stmt execute(std::string_view query, const BindValue& value) const;
		Stmt execute(std::string_view query, std::initializer_list<const BindValue> values) const;
		
		int count_rows(std::string_view table) const;
		std::vector<std::vector<ScalarValue>> query_all(std::string_view table, int failsafeLimit = 10000) const;
		
		
	public:
		const std::string& path() const;
		
		
	public:
		inline const std::shared_ptr<DB>& db() { return m_db; }
	};
}
