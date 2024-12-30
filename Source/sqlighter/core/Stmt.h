#ifndef SQLIGHTER_STMT_H
#define SQLIGHTER_STMT_H


#include <memory>
#include <sqlite3.h>


namespace sqlighter
{
	class Stmt
	{
	private:
		std::shared_ptr<sqlite3_stmt> m_stmt;
	};
}


#endif
