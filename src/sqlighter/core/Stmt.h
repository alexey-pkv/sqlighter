#ifndef SQLIGHTER_CORE_STMT_H
#define SQLIGHTER_CORE_STMT_H


#include <string>
#include <cstdint>
#include <sqlite3.h>
#include <vector>
#include <map>
#include <memory>


#include "StmtRef.h"
#include "ScalarValue.h"


namespace sqlighter
{
	class DB;
	
	class Stmt
	{
	private:
		StmtRef m_stmt = {};
		
		mutable std::vector<std::string> 	m_columns		= {};
		mutable std::map<std::string, int>	m_columnIndex	= {};
		
		std::string 		m_query		= {};
		std::shared_ptr<DB> m_db		= nullptr;
		int 				m_lastCode	= SQLITE_ERROR;
		
		
	private:
		ScalarValue to_value(int at) const;
		
		
	public:
		~Stmt();
		
		Stmt() = default;
		explicit Stmt(sqlite3_stmt* stmt);
		
		Stmt(sqlite3_stmt* stmt, std::shared_ptr<DB> db);
		Stmt(std::shared_ptr<DB> db, std::string_view query);
		
		Stmt(const Stmt&) = delete;
		Stmt& operator=(const Stmt&) = delete;
		
		Stmt(Stmt&&) noexcept;
		Stmt& operator=(Stmt&&) noexcept;
		
		
	public:
		inline StmtRef& stmt_ref() noexcept { return m_stmt; }
		inline int code() const { return m_lastCode; }
		
		inline std::shared_ptr<const DB> db() const { return m_db; }
		inline std::shared_ptr<DB> db() { return m_db; }
		
		inline bool is_done() const		{ return m_lastCode == SQLITE_DONE;		}
		inline bool is_ok() const		{ return m_lastCode == SQLITE_OK;		}
		inline bool is_error() const	{ return m_lastCode == SQLITE_ERROR;	}
		inline bool has_row() const		{ return m_lastCode == SQLITE_ROW;		}
		inline bool is_closed() const	{ return !m_stmt; }
		
		inline const std::string& query() const { return m_query; }
		
		
	public:
		int step();
		sqlite3_stmt* stmt();
		const sqlite3_stmt* stmt() const;
		
		
		void close();
		
		/**
		 * Throw an exception if the stmt was finalized and closed.
		 */
		void require_open() const;
		
		/**
		 * Throw an exception if there is no current row to read form.
		 */
		void require_row() const;
		
		/**
		 * Ensure column given at position exists.  
		 */
		void require_column(int at) const;
		
		/**
		 * Ensure column given at position exists.  
		 */
		void require_column_type(int at, int type) const;
		
		
	public:
		int column_int(int at) const;
		int64_t column_int64(int at) const;
		bool column_bool(int at) const;
		double column_double(int at) const;
		std::string column_string(int at) const;
		size_t column_blob(int at, void** into) const;
		blob_t column_blob(int at) const;
		bool column_is_null(int at) const;
		
		bool column_int_n(int at, int& into) const;
		bool column_int64_n(int at, int64_t& into) const;
		bool column_bool_n(int at, bool& into) const;
		bool column_double_n(int at, double& into) const;
		bool column_string_n(int at, std::string& into) const;
		
		ScalarValue column_value(int at) const;
		ScalarValue column_value_and_step(int at);
		std::vector<ScalarValue> row() const;
		std::vector<ScalarValue> row_and_step();
		std::vector<std::vector<ScalarValue>> all(int failsafeLimit);
		
		const char* column_name(int at) const;
		const std::string& column_name_str(int at) const;
		const std::vector<std::string>& column_names() const;
		int column_type(int at) const;
		int column_count() const;
		int column_index(std::string_view name) const;
		bool has_column(std::string_view name) const;
		
		void require_one_column() const;
		void require_done() const;
		
		
	public:
		template <typename T, typename = std::enable_if_t<
			std::is_same<T, bool>::value ||
			std::is_integral<T>::value ||
			std::is_floating_point<T>::value ||
			std::is_same<T, std::string>::value
		>>
		T column(int at) const;
		
		template <typename T, typename = std::enable_if_t<
			std::is_same<T, bool>::value ||
			std::is_integral<T>::value ||
			std::is_floating_point<T>::value ||
			std::is_same<T, std::string>::value
		>>
		bool column_n(int at, T& into) const;
	};
}


#endif
