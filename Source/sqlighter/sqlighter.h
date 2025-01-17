#pragma once


#include "connectors/CMDDrop.h"
#include "connectors/CMDSelect.h"
#include "connectors/CMDInsert.h"
#include "connectors/CMDDirect.h"
#include "connectors/CMDDelete.h"
#include "connection/Connection.h"
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
		explicit SQLighter(const char* db_path);
		explicit SQLighter(const std::shared_ptr<DB>& db);
		
		
	public:
		CMDDirect		direct() const;
		CMDSelect		select() const;
		CMDInsert		insert() const;
		CMDDelete		del() const;
		CMDCreateTable	create() const;
		CMDDrop			drop() const;
		
		Stmt execute(std::string_view query) const;
		Stmt execute(std::string_view query, const BindValue& value) const;
		Stmt execute(std::string_view query, std::initializer_list<BindValue> values) const;
		
		int count_rows(std::string_view table) const;
		std::vector<std::vector<ScalarValue>> query_all(std::string_view table, int failsafeLimit = 10000) const;
		
		
	public:
		inline void begin() const		{ execute("BEGIN"); }
		inline void commit() const		{ execute("COMMIT"); }
		inline void rollback() const	{ execute("ROLLBACK"); }
		
		
	public:
		void reindex(std::string_view element) const;
		void reindex(std::string_view scheme, std::string_view element) const;
		
		
	public:
		const std::string& path() const;
		
		
	public:
		inline const std::shared_ptr<DB>& db() { return m_db; }
	};
}
