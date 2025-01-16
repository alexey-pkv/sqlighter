#include "exceptions/error_message.h"


#include "exceptions/error_codes.h"


#include <gtest/gtest.h>


TEST(error_message, message_defined)
{
	// Part of the Not Defined String that will be returned for invalid error message 
	const char* NDS = "is not an sqlighter code";
	
	
	ASSERT_NE(std::string::npos, sqlighter::error_message(1).find(NDS));
	
	
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_OK).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_GENERIC).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_UNEXPECTED).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_STMT_FINALIZED).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_STEP).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_PREPARE).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_NO_ROWS).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_NO_COLUMN).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_MULT_COLUMNS).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_INVALID_COLUMN).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_BIND).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_VALUE).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_FAILED_TO_CLOSE_DB).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_FAILED_TO_OPEN_DB).find(NDS));
	ASSERT_EQ(std::string::npos, sqlighter::error_message(SQLIGHTER_ERR_INVALID_ENUM).find(NDS));
}