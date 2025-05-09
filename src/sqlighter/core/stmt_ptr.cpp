#include "stmt_ptr.h"


using namespace std;
using namespace sqlighter;


shared_stmt_ptr::shared_stmt_ptr(sqlite3_stmt* ptr) : 
	std::shared_ptr<sqlite3_stmt>(ptr, Sqlite3StmtDeleter())
{
	
}
