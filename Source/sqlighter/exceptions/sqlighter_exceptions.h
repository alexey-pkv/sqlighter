#pragma once


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
		
		mutable std::string m_what {};
		
		
	public:
		~SQLighterException() = default;
		SQLighterException() = delete;
		
		SQLighterException(SQLighterException&&) = default;
		SQLighterException(const SQLighterException&) = default;
		SQLighterException& operator=(SQLighterException&&) = default;
		SQLighterException& operator=(const SQLighterException&) = default;
		
		SQLighterException(int code);
		SQLighterException(int code, int sqlite_code);
		SQLighterException(int code, int sqlite_code, std::string_view message);
		SQLighterException(int code, std::string_view message);
		SQLighterException(int code, const std::shared_ptr<const DB>& db);
		
		
	public:
		inline int code() const noexcept { return m_lighter_code; }
		inline int sqlite_code() const noexcept { return m_sqlite_code; }
		inline bool has_sqlite_code() const noexcept { return m_sqlite_code != -1; }
		inline bool has_message() const noexcept { return !m_message.empty(); }
		
		
	public:
		const std::string& message() const noexcept; 
		const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
		const char* what_sqlite3() const;
		const std::string& what_str() const;
		
		
	public:
		static SQLighterException invalid_enum(const char* enum_name, int value);
		static SQLighterException failed_to_bind(int sqlite_code, int offset, const BindValue& bv);
	};
}
