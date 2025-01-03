#pragma once


#include <exception>

#include "core/Stmt.h"


namespace sqlighter
{
	class SQLighterException : public std::exception
	{
		
	};
	
	
	class StmtFinalizedException : public SQLighterException
	{
		
	};
	
	
	class SQLighterStatementException : public SQLighterException
	{
	private:
		Stmt m_stmt;
		
		
	public:
		explicit SQLighterStatementException(Stmt&& stmt);
		
		SQLighterStatementException(SQLighterStatementException&&) = default;
		SQLighterStatementException& operator=(SQLighterStatementException&&) = default;
		
		SQLighterStatementException() = delete;
		SQLighterStatementException(const SQLighterStatementException&) = delete;
		SQLighterStatementException operator=(const SQLighterStatementException&) = delete;
		
		
	public:
		[[nodiscard]] inline int error_code() const { return m_stmt.code(); }
	};
	
	class SQLighterNorRowToRead : public SQLighterException
	{
		
	};
}
