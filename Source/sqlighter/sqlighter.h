#ifndef SQLIGHTER_SQLIGHTER_H
#define SQLIGHTER_SQLIGHTER_H


#include "sqlighter_version.h"

#include "connectors/CMDDrop.h"
#include "connectors/CMDSelect.h"
#include "connectors/CMDInsert.h"
#include "connectors/CMDDirect.h"
#include "connectors/CMDDelete.h"
#include "connectors/CMDUpdate.h"
#include "connection/Connection.h"
#include "connectors/CMDCreateTable.h"


namespace sqlighter
{
	class SQLighter
	{
	private:
		std::string 				m_path;
		std::shared_ptr<DB>			m_db			{ nullptr };
		std::shared_ptr<Connection>	m_connection	{ nullptr };
		
		
	private:
		const std::shared_ptr<Connection>& connection();
		
		
	public:
		~SQLighter() = default;
		SQLighter() = delete;
		
		SQLighter(SQLighter&&) noexcept = default;
		SQLighter(const SQLighter&) noexcept = default;
		SQLighter& operator=(SQLighter&&) noexcept = default;
		SQLighter& operator=(const SQLighter&) noexcept = default;
		
		explicit SQLighter(const std::filesystem::path& db_path);
		explicit SQLighter(const char* db_path);
		explicit SQLighter(const std::shared_ptr<DB>& db);
		
		
	public:
		CMDDirect		direct();
		CMDSelect		select();
		CMDInsert		insert();
		CMDUpdate		update();
		CMDDelete		del();
		CMDCreateTable	create();
		CMDDrop			drop();
		
		Stmt execute(std::string_view query);
		Stmt execute(std::string_view query, const BindValue& value);
		Stmt execute(std::string_view query, std::initializer_list<BindValue> values);
		
		int64_t count_rows(std::string_view table);
		std::vector<std::vector<ScalarValue>> query_all(std::string_view table, int failsafeLimit = 10000);
		
		
	public:
		inline void begin()		{ execute("BEGIN"); }
		inline void commit()		{ execute("COMMIT"); }
		inline void rollback()	{ execute("ROLLBACK"); }
		
		
	public:
		void reindex(std::string_view element);
		void reindex(std::string_view scheme, std::string_view element);
		std::string sqlite_version();
		
		
	public:
		const std::string& path() const;
		void close();
		void open();
		bool is_open() const noexcept; 
		
		
	public:
		inline const std::shared_ptr<DB>& db() { return m_db; }
		inline static SQLighter memory() { return std::move(SQLighter(":memory:")); }
	};
	
	
	std::string sqlite_lib_version();
	std::string sqlighter_lib_version();
}


#endif
