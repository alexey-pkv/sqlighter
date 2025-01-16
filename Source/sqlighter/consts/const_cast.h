#pragma once


#include "OrderBy.h"
#include "core/ScalarValue.h"


#include <sstream>


namespace sqlighter
{
	std::ostream& operator<<(std::ostream& os, sqlighter::OrderBy order);
	std::ostream& operator<<(std::ostream& os, sqlighter::ScalarValue::type type);
}
