#pragma once


#include <string>
#include <cstdint>
#include <sqlite3.h>


namespace sqlighter
{
	class Stmt
	{
	private:
		sqlite3_stmt*	m_stmt		= nullptr;
		int 			m_lastCode	= SQLITE_ERROR;
		
		
	public:
		~Stmt();
		
		Stmt() = default;
		explicit Stmt(sqlite3_stmt* stmt);
		
		Stmt(const Stmt&) = delete;
		Stmt& operator=(const Stmt&) = delete;
		
		Stmt(Stmt&&) = default;
		Stmt& operator=(Stmt&&) = default;
		
		
	public:
		inline sqlite3_stmt** stmt_p() { return &m_stmt; }
		inline int code() const { return m_lastCode; }
		
		inline int is_done() const	{ return m_lastCode == SQLITE_DONE;		}
		inline int is_ok() const	{ return m_lastCode == SQLITE_OK;		}
		inline int is_error() const	{ return m_lastCode == SQLITE_ERROR;	}
		inline int hsa_row() const	{ return m_lastCode == SQLITE_ROW;		}
		
		
	public:
		int step();
		sqlite3_stmt* stmt();
		const sqlite3_stmt* stmt() const;
		
		
		int close();
		
		/**
		 * Throw an exception if there is no current row to read form.
		 */
		void require_row() const;
		
		
	public:
		int column_int(int at) const;
		int64_t column_int64(int at) const;
		bool column_bool(int at) const;
		double column_double(int at) const;
		std::string column_string(int at) const;
		size_t column_blob(int at, void** into) const;
		
		int column_type(int at) const;
		int column_count(int at) const;
	};
}

