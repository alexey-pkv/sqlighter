#include "const_cast.h"


#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


std::ostream& sqlighter::operator<<(std::ostream& os, OrderBy order)
{
	switch (order)
	{
		case OrderBy::ASC:		os << "ASC";	break;
		case OrderBy::DESC:		os << "DESC";	break;
			
		default:
			throw SQLighterException::invalid_enum("OrderBy", (int)order);
	}
	
	return os;
}
