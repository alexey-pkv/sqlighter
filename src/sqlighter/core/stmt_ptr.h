#ifndef SQLIGHTER_CORE_UNIQUE_STMT_PTR_H
#define SQLIGHTER_CORE_UNIQUE_STMT_PTR_H


#include <memory>
#include <sqlite3.h>


namespace sqlighter
{
	struct Sqlite3StmtDeleter 
	{
		inline void operator()(sqlite3_stmt* stmt) const 
		{
			if (stmt)
			{
				sqlite3_finalize(stmt);
			}
		}
	};
	
	
	using unique_stmt_ptr	= std::unique_ptr<sqlite3_stmt, Sqlite3StmtDeleter>;
	using weak_stmt_ptr		= std::weak_ptr<sqlite3_stmt>;
	
	class shared_stmt_ptr : public std::shared_ptr<sqlite3_stmt>
	{
	public:
		shared_stmt_ptr(sqlite3_stmt* ptr);
		
		shared_stmt_ptr() = default;
		shared_stmt_ptr(const shared_stmt_ptr&) = default;
        shared_stmt_ptr(shared_stmt_ptr&&) = default;
		shared_stmt_ptr& operator=(const shared_stmt_ptr&) = default;
        shared_stmt_ptr& operator=(shared_stmt_ptr&&) = default;
	};
}


#endif
