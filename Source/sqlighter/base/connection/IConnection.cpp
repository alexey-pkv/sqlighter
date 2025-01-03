#include "IConnection.h"


#include "core/Stmt.h"


using namespace sqlighter;


AbstractConnectionDecorator::AbstractConnectionDecorator(const std::shared_ptr<IConnection>& next) : 
	m_next(next)
{
	if (next == nullptr)
	{
		throw std::runtime_error("Connection can not be null!");
	}
}


Stmt AbstractConnectionDecorator::execute_next(std::string_view query, const std::vector<BindValue>& values)
{
	return next()->execute(query, values);
}
