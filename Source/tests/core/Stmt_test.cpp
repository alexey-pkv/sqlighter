#include "core/Stmt.h"


#include <fstream>
#include <gtest/gtest.h>

#include "sqlighter.h"
#include "../test_core/db_mock.h"


using namespace sqlighter;


Stmt stmt__select_mock(const SQLighter& sql)
{
	auto select = sql.select();
	select->from("mock_table");
	
	return select->execute();
}


TEST(Stmt, column_bool__true)
{
	SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", 1) };
	auto stmt = stmt__select_mock(sql);
	
	
	ASSERT_TRUE(stmt.column_bool(1));
}

TEST(Stmt, column_bool__false)
{
	SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", 0) };
	auto stmt = stmt__select_mock(sql);
	
	
	ASSERT_FALSE(stmt.column_bool(1));
}

TEST(Stmt, column_int)
{
	SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", 12) };
	auto stmt = stmt__select_mock(sql);
	
	
	ASSERT_EQ(12, stmt.column_int(1));
}

TEST(Stmt, column_int64)
{
	SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", INT64_MAX) };
	auto stmt = stmt__select_mock(sql);
	
	
	ASSERT_EQ(INT64_MAX, stmt.column_int64(1));
}

TEST(Stmt, column_double)
{
	SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", 1, "c_dlb", 123.213) };
	auto stmt = stmt__select_mock(sql);
	
	
	ASSERT_EQ(123.213, stmt.column_double(2));
}

TEST(Stmt, column_string)
{
	SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", 1, "c_dlb", 1, "c_str", "hello world") };
	auto stmt = stmt__select_mock(sql);
	
	
	ASSERT_EQ("hello world", stmt.column_string(3));
}

TEST(Stmt, column_count)
{
	SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", 1, "c_dlb", 1, "c_str", "hello world") };
	auto stmt = stmt__select_mock(sql);
	
	
	ASSERT_EQ(4, stmt.column_count());
}

TEST(Stmt, column_type)
{
	SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", 1, "c_dlb", 1, "c_str", "hello world") };
	auto stmt = stmt__select_mock(sql);
	
	ASSERT_EQ(SQLITE_NULL,		stmt.column_type(0));
	ASSERT_EQ(SQLITE_INTEGER,	stmt.column_type(1));
	ASSERT_EQ(SQLITE_FLOAT,		stmt.column_type(2));
	ASSERT_EQ(SQLITE_TEXT,		stmt.column_type(3));
}