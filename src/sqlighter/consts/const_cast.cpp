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


std::ostream& sqlighter::operator<<(std::ostream& os, ScalarValue::type t)
{
	switch (t)
	{
		case ScalarValue::type::INT:		os << "INT_64";		break;
		case ScalarValue::type::DOUBLE:		os << "DOUBLE";		break;
		case ScalarValue::type::NULL_VAL:	os << "NULL_VAL";	break;
		case ScalarValue::type::TEXT:		os << "TEXT";		break;
		case ScalarValue::type::BLOB:		os << "BLOB";		break;
		
		default:
			throw SQLighterException::invalid_enum("ScalarValue::type", (int)t);
	}
	
	return os;
}
