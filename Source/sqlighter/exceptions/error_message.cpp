#include "error_message.h"


#include "error_codes.h"


std::string sqlighter::error_message(int sqlighter_code)
{
	switch (sqlighter_code)
	{
		case SQLIGHTER_OK:			return "OK";
		case SQLIGHTER_ERR_GENERIC: return "Generic error";
		
		case SQLIGHTER_ERR_STMT_FINALIZED:	return "";
		case SQLIGHTER_ERR_STEP:			return "Failed to execute step";
		case SQLIGHTER_ERR_PREPARE:			return "Error when preparing statement";
		case SQLIGHTER_ERR_NO_ROWS:			return "A row is required for this operation. The current statement does not hold a row.";
		case SQLIGHTER_ERR_BIND:			return "Failed to bind a value";
		
		case SQLIGHTER_ERR_FAILED_TO_CLOSE_DB:	return "Error when closing DB";
		case SQLIGHTER_ERR_FAILED_TO_OPEN_DB:	return "Error when opening DB";
		
		case SQLIGHTER_ERR_INVALID_ENUM: return "Invalid enum value encountered";
	}
	
	return std::to_string(sqlighter_code) + " is not an sqlighter code.";
}

