#include "IConnection.h"


#include "core/Stmt.h"
#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


AbstractConnectionDecorator::AbstractConnectionDecorator(const std::shared_ptr<IConnection>& next) : 
	m_next(next)
{
	if (next == nullptr)
	{
		throw SQLighterException(SQLIGHTER_ERR_UNEXPECTED, "Connection can not be null!");
	}
}


Stmt AbstractConnectionDecorator::execute_next(std::string_view query, const std::vector<BindValue>& values)
{
	return next()->execute(query, values);
}
