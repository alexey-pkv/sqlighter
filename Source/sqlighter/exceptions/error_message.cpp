#include "error_message.h"


#include "error_codes.h"


std::string sqlighter::error_message(int sqlighter_code)
{
	switch (sqlighter_code)
	{
		case SQLIGHTER_OK:			return "OK";
		case SQLIGHTER_ERR_NO_ROWS:	return "A row is required for this operation. The current statement does not hold a row.";	
	}
	
	return std::to_string(sqlighter_code) + " is not an sqlighter code.";
}
