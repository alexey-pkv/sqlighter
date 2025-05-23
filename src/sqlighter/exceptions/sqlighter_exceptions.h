#ifndef SQLIGHTER_EXCEPTIONS_SQLIGHTER_EXCEPTIONS_H
#define SQLIGHTER_EXCEPTIONS_SQLIGHTER_EXCEPTIONS_H


#include <memory>
#include <exception>

#include "core/DB.h"
#include "core/Stmt.h"
#include "core/BindValue.h"

// Not used directly in this file, but if the exception class is included, error code should be also.
#include "error_codes.h"


namespace sqlighter
{
	class SQLighterException : public std::exception
	{
	private:
		int m_sqlite_code = -1;
		int m_lighter_code;
		
		std::string m_message {};
		std::string m_query {};
		
		mutable std::string m_what {};
		
		
	public:
		~SQLighterException() override = default; // LCOV_EXCL_LINE
		SQLighterException() = delete;
		
		SQLighterException(SQLighterException&&) noexcept = default;
		SQLighterException(const SQLighterException&) noexcept = default;
		SQLighterException& operator=(SQLighterException&&) noexcept = default;
		SQLighterException& operator=(const SQLighterException&) noexcept = default;
		
		explicit SQLighterException(int code) noexcept;
		
		SQLighterException(int code, int sqlite_code);
		SQLighterException(int code, int sqlite_code, std::string_view message);
		SQLighterException(int code, std::string_view message);
		
		
	public:
		inline int code() const noexcept { return m_lighter_code; }
		inline int sqlite_code() const noexcept { return m_sqlite_code; }
		inline bool has_sqlite_code() const noexcept { return m_sqlite_code != -1; }
		inline bool has_message() const noexcept { return !m_message.empty(); }
		inline bool has_query() const noexcept { return !m_query.empty(); } 
		inline const std::string& query() const noexcept { return m_query; }
		
		
	public:
		inline SQLighterException& msg(std::string_view message) { m_message = message; return *this; }
		inline SQLighterException& sqlite_error(int code) { m_sqlite_code = code; return *this; }
		inline SQLighterException& db_msg(const std::shared_ptr<const DB>& db) { m_message = db->error_message(); return *this; }
		inline SQLighterException& query(std::string_view query) { m_query = query; return *this; }
		
		
	public:
		const std::string& message() const noexcept;
		const char* what() const noexcept override;
		const char* what_sqlite3() const;
		const std::string& what_str() const;
		
		
	public:
		static SQLighterException invalid_enum(const char* enum_name, int value);
		static SQLighterException failed_to_bind(int sqlite_code, int offset, const BindValue& bv);
		static SQLighterException no_column(int at, int count, std::string_view query);
	};
}


#endif
