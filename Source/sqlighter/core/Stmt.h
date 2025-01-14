#pragma once


#include <string>
#include <cstdint>
#include <sqlite3.h>
#include <vector>
#include <map>
#include <memory>


namespace sqlighter
{
	class DB;
	
	class Stmt
	{
	private:
		sqlite3_stmt*						m_stmt		= nullptr;
		
		mutable std::vector<std::string> 	m_columns		= {};
		mutable std::map<std::string, int>	m_columnIndex	= {};
		
		std::string 		m_query		= {};
		std::shared_ptr<DB> m_db		= nullptr;
		int 				m_lastCode	= SQLITE_ERROR;
		
		
	public:
		~Stmt();
		
		Stmt() = default;
		explicit Stmt(std::shared_ptr<DB> db);
		explicit Stmt(sqlite3_stmt* stmt);
		
		Stmt(sqlite3_stmt* stmt, std::shared_ptr<DB> db);
		Stmt(std::shared_ptr<DB> db, std::string_view query);
		
		Stmt(const Stmt&) = delete;
		Stmt& operator=(const Stmt&) = delete;
		
		Stmt(Stmt&&) = default;
		Stmt& operator=(Stmt&&) = default;
		
		
	public:
		inline sqlite3_stmt** stmt_p() { return &m_stmt; }
		inline int code() const { return m_lastCode; }
		
		inline std::shared_ptr<const DB> db() const { return m_db; }
		inline std::shared_ptr<DB> db() { return m_db; }
		
		inline int is_done() const	{ return m_lastCode == SQLITE_DONE;		}
		inline int is_ok() const	{ return m_lastCode == SQLITE_OK;		}
		inline int is_error() const	{ return m_lastCode == SQLITE_ERROR;	}
		inline int has_row() const	{ return m_lastCode == SQLITE_ROW;		}
		
		inline const std::string& query() const { return m_query; }
		
		
	public:
		int step();
		sqlite3_stmt* stmt();
		const sqlite3_stmt* stmt() const;
		
		
		int close();
		
		/**
		 * Throw an exception if there is no current row to read form.
		 */
		void require_row() const;
		
		/**
		 * Ensure column given at position exists.  
		 */
		void require_column(int at) const;
		
		
	public:
		int column_int(int at) const;
		int64_t column_int64(int at) const;
		bool column_bool(int at) const;
		double column_double(int at) const;
		std::string column_string(int at) const;
		size_t column_blob(int at, void** into) const;
		bool column_is_null(int at) const;
		
		bool column_int_n(int at, int& into) const;
		bool column_int64_n(int at, int64_t& into) const;
		bool column_bool_n(int at, bool& into) const;
		bool column_double_n(int at, double& into) const;
		bool column_string_n(int at, std::string& into) const;
		
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
		template <typename T>
		requires
			std::same_as<T, bool>			||
			std::is_integral_v<T>			||
			std::is_floating_point_v<T>		||
			std::same_as<T, std::string>
		T column(int at) const;
		
		template <typename T>
		requires
			std::same_as<T, bool>			||
			std::is_integral_v<T>			||
			std::is_floating_point_v<T>		||
			std::same_as<T, std::string>
		bool column_n(int at, T& into) const;
	};
}

