#pragma once


#include "OrderBy.h"


#include <sstream>


namespace sqlighter
{
	std::ostream& operator<<(std::ostream& os, sqlighter::OrderBy order);
}
