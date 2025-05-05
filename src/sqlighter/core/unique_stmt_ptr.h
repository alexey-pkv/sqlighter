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
	
	
	using unique_stmt_ptr = std::unique_ptr<sqlite3_stmt, Sqlite3StmtDeleter>;
}


#endif
