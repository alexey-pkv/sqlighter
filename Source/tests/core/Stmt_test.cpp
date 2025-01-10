#include "core/Stmt.h"


#include <fstream>
#include <gtest/gtest.h>

#include "sqlighter.h"
#include "../test_core/db_mock.h"


using namespace sqlighter;


Stmt stmt__select_mock(const SQLighter& sql, std::string_view table = "mock_table")
{
	auto select = sql.select();
	select->from(table);
	
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


TEST(Stmt, column_bool_n)
{
	SQLighter sql { setup_db("test_mock_n.db") };
	
	sql.execute(
		"CREATE TABLE test_mock ("
			"c_null BOOL NULL,"
			"c_true BOOL NULL,"
			"c_false BOOL NULL"
		")");
	
	sql.execute(
		"INSERT INTO test_mock (c_null, c_true, c_false) VALUES (NULL, ?, ?)",
		{ true, false }
	);
	
	
	auto stmt = stmt__select_mock(sql, "test_mock");
	bool value;
	
	
	value = true;
	ASSERT_FALSE(stmt.column_bool_n(0, value));
	ASSERT_FALSE(value);
	
	value = false;
	ASSERT_TRUE(stmt.column_bool_n(1, value));
	ASSERT_TRUE(value);
	
	value = true;
	ASSERT_TRUE(stmt.column_bool_n(2, value));
	ASSERT_FALSE(value);
}

TEST(Stmt, column_int_n)
{
	SQLighter sql { setup_db("test_mock_n.db") };
	
	sql.execute(
		"CREATE TABLE test_mock ("
			"c_null INT NULL,"
			"c_value INT NULL"
		")");
	
	sql.execute(
		"INSERT INTO test_mock (c_null, c_value) VALUES (NULL, ?)",
		{ 123 }
	);
	
	
	auto stmt = stmt__select_mock(sql, "test_mock");
	int value;
	
	
	value = 11;
	ASSERT_FALSE(stmt.column_int_n(0, value));
	ASSERT_EQ(0, value);
	
	value = 0;
	ASSERT_TRUE(stmt.column_int_n(1, value));
	ASSERT_EQ(123, value);
}

TEST(Stmt, column_int64_n)
{
	SQLighter sql { setup_db("test_mock_n.db") };
	
	sql.execute(
		"CREATE TABLE test_mock ("
			"c_null INT NULL,"
			"c_value INT NULL"
		")");
	
	sql.execute(
		"INSERT INTO test_mock (c_null, c_value) VALUES (NULL, ?)",
		{ INT64_MAX }
	);
	
	
	auto stmt = stmt__select_mock(sql, "test_mock");
	int64_t value;
	
	
	value = 11;
	ASSERT_FALSE(stmt.column_int64_n(0, value));
	ASSERT_EQ(0, value);
	
	value = 0;
	ASSERT_TRUE(stmt.column_int64_n(1, value));
	ASSERT_EQ(INT64_MAX, value);
}

TEST(Stmt, column_double_n)
{
	SQLighter sql { setup_db("test_mock_n.db") };
	
	sql.execute(
		"CREATE TABLE test_mock ("
			"c_null REAL NULL,"
			"c_value REAL NULL"
		")");
	
	sql.execute(
		"INSERT INTO test_mock (c_null, c_value) VALUES (NULL, ?)",
		{ 123.321 }
	);
	
	
	auto stmt = stmt__select_mock(sql, "test_mock");
	double value;
	
	
	value = 11;
	ASSERT_FALSE(stmt.column_double_n(0, value));
	ASSERT_EQ(0, value);
	
	value = 0;
	ASSERT_TRUE(stmt.column_double_n(1, value));
	ASSERT_EQ(123.321, value);
}

TEST(Stmt, column_string_n)
{
	SQLighter sql { setup_db("test_mock_n.db") };
	
	sql.execute(
		"CREATE TABLE test_mock ("
			"c_null TEXT NULL,"
			"c_value TEXT NULL"
		")");
	
	sql.execute(
		"INSERT INTO test_mock (c_null, c_value) VALUES (NULL, ?)",
		{ "hello world" }
	);
	
	
	auto stmt = stmt__select_mock(sql, "test_mock");
	std::string value;
	
	
	value = "dummy value";
	ASSERT_FALSE(stmt.column_string_n(0, value));
	ASSERT_TRUE(value.empty());
	
	value = "dummy value";
	ASSERT_TRUE(stmt.column_string_n(1, value));
	ASSERT_EQ("hello world", value);
}