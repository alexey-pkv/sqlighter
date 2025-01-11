#include "exceptions/sqlighter_exceptions.h"


#include "sqlighter.h"
#include "db_mock.h"
#include "exceptions/error_message.h"


#include <sqlite3.h>
#include <gtest/gtest.h>


using namespace sqlighter;

/*
SQLighterException(int code);
		SQLighterException(int code, int sqlite_code);
		SQLighterException(int code, int sqlite_code, std::string_view message);
		SQLighterException(int code, std::string_view message);
		SQLighterException(int code, const std::shared_ptr<const DB>& db);
		SQLighterException(int code, int sqlite_code, const std::shared_ptr<const DB>& db);*/


TEST(SQLighterException, constructor__int)
{
	SQLighterException e(123);
	
	
	ASSERT_EQ(123,	e.code());
	ASSERT_EQ(-1,	e.sqlite_code());
	ASSERT_EQ("",	e.message());
}

TEST(SQLighterException, constructor__int_int)
{
	SQLighterException e(123, 12);
	
	
	ASSERT_EQ(123,	e.code());
	ASSERT_EQ(12,	e.sqlite_code());
	ASSERT_EQ("",	e.message());
}

TEST(SQLighterException, constructor__int_int_str)
{
	SQLighterException e(123, 12, "hello");
	
	
	ASSERT_EQ(123,		e.code());
	ASSERT_EQ(12,		e.sqlite_code());
	ASSERT_EQ("hello",	e.message());
}

TEST(SQLighterException, constructor__int_str)
{
	SQLighterException e(123, "hello");
	
	
	ASSERT_EQ(123,		e.code());
	ASSERT_EQ(-1,		e.sqlite_code());
	ASSERT_EQ("hello",	e.message());
}

TEST(SQLighterException, constructor__int_db)
{
	SQLighter sql { setup_db("test_mock_n.db") };
	
	try { sql.execute("abc"); } catch (const std::exception& e){}
	
	
	SQLighterException e(123, sql.db());
	
	ASSERT_EQ(123,	e.code());
	ASSERT_EQ(-1,	e.sqlite_code());
	
	ASSERT_FALSE(e.message().empty());
}

TEST(SQLighterException, constructor__int_int_db)
{
	SQLighter sql { setup_db("test_mock_n.db") };
	try { sql.execute("abc"); } catch (const std::exception& e){}
	
	
	SQLighterException e(123, 12, sql.db());
	
	
	ASSERT_EQ(123,	e.code());
	ASSERT_EQ(12,	e.sqlite_code());
	
	ASSERT_FALSE(e.message().empty());
}


TEST(SQLighterException, what_str__sanity)
{
	SQLighterException e(123, 12);
	
	
	ASSERT_EQ(e.what_str(), e.what());
}

TEST(SQLighterException, what__sanity)
{
	SQLighter sql { setup_db("test_mock_n.db") };
	try { sql.execute("abc"); } catch (const std::exception& e){}
	
	SQLighterException e(SQLIGHTER_ERR_STEP, SQLITE_NOTFOUND, sql.db());
	
	
	std::string str = e.what();
	
	
	ASSERT_FALSE(str.empty());
	
	ASSERT_NE(std::string::npos, str.find("unknown operation"));
	ASSERT_NE(std::string::npos, str.find("syntax error"));
	ASSERT_NE(std::string::npos, str.find("Failed to execute step"));
}

TEST(SQLighterException, what__message_only)
{
	SQLighterException e(SQLIGHTER_ERR_GENERIC, "hello world");
	
	
	std::string str = e.what();
	
	
	ASSERT_FALSE(str.empty());
	
	ASSERT_NE(std::string::npos, str.find("hello world"));
}

TEST(SQLighterException, what__code_only)
{
	SQLighterException e(SQLIGHTER_ERR_GENERIC);
	
	
	std::string str = e.what();
	
	
	ASSERT_EQ(error_message(SQLIGHTER_ERR_GENERIC), str);
}

TEST(SQLighterException, what_sqlite3__no_error__nullptr_returned)
{
	SQLighterException e(SQLIGHTER_ERR_GENERIC);
	
	
	ASSERT_EQ(nullptr, e.what_sqlite3());
}

TEST(SQLighterException, what_sqlite3__has_error__error_parsed)
{
	SQLighterException e(SQLIGHTER_ERR_GENERIC, SQLITE_NOTFOUND);
	
	
	std::string str = e.what_sqlite3();
	
	
	ASSERT_EQ("unknown operation", str);
}