#ifndef SQLIGHTER_CONSTS_CONST_CAST_H
#define SQLIGHTER_CONSTS_CONST_CAST_H


#include "OrderBy.h"
#include "core/ScalarValue.h"


#include <sstream>


namespace sqlighter
{
	std::ostream& operator<<(std::ostream& os, sqlighter::OrderBy order);
	std::ostream& operator<<(std::ostream& os, sqlighter::ScalarValue::type type);
}


#endif
