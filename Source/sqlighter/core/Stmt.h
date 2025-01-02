#pragma once


#include <sqlite3.h>


namespace sqlighter
{
	class Stmt
	{
	private:
		sqlite3_stmt*	m_stmt = nullptr;
		
		
	public:
		~Stmt();
		explicit Stmt(sqlite3_stmt* stmt);
		
		Stmt() = delete;
		Stmt(const Stmt&) = delete;
		Stmt& operator=(const Stmt&) = delete;
		
		
		
		
	public:
		[[nodiscard]] sqlite3_stmt* stmt();
		[[nodiscard]] const sqlite3_stmt* stmt() const;
		
		
		int close();
	};
}

