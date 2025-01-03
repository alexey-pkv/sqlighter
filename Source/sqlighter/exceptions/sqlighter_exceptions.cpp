#include "sqlighter_exceptions.h"


#include <utility>


using namespace sqlighter;


SQLighterStatementException::SQLighterStatementException(Stmt&& stmt) :
	m_stmt(std::move(stmt))
{
	
}
