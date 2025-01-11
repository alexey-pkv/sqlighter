#include "core/Stmt.h"


#include <fstream>

#include <gtest/gtest.h>
#include <cmath>

#include "sqlighter.h"
#include "../test_core/db_mock.h"

#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


Stmt stmt__select_mock(const SQLighter& sql, std::string_view table = "mock_table")
{
	return sql
		.select()
		.from(table)
		.execute();
}


TEST(Stmt, stmt)
{
	SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", 1) };
	auto stmt = stmt__select_mock(sql);
	
	
	ASSERT_NE(nullptr, stmt.stmt());
	ASSERT_NE(nullptr, static_cast<const Stmt&>(stmt).stmt());
}

TEST(Stmt, contructor_1)
{
	Stmt stmt(nullptr, nullptr);
	
	ASSERT_EQ(nullptr, stmt.db());
	ASSERT_EQ(nullptr, static_cast<const Stmt&>(stmt).db());
}

TEST(Stmt, stmt__no_stmt__exception_thrown)
{
	try
	{
		Stmt s(nullptr);
		
		s.stmt();
		
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_STMT_FINALIZED, e.code());
	}
}

TEST(Stmt, const_stmt__no_stmt__exception_thrown)
{
	try
	{
		const Stmt s(nullptr);
		
		s.stmt();
		
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_STMT_FINALIZED, e.code());
	}
}

TEST(Stmt, step__no_stmt__exception_thrown)
{
	try
	{
		Stmt s(nullptr);
		
		s.step();
		
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_STMT_FINALIZED, e.code());
	}
}

TEST(Stmt, step__step_failed__excpetion_thrown)
{
	SQLighter sql { setup_db("test_mock_n.db") };
	
	try
	{
		Stmt stmt = sql.execute(
			"CREATE TABLE test_mock ("
				"c_null TEXT NULL,"
				"c_value TEXT NULL"
			")");
		
		stmt.step();
		
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_STEP, e.code());
		ASSERT_EQ(SQLITE_ERROR, e.sqlite_code());
	}
}

TEST(Stmt, require_row__have_rows__no_error)
{
	SQLighter sql { create_mock_table("my_table") };
	Stmt stmt = sql.execute("SELECT * FROM my_table");
	
	
	stmt.require_row();
}

TEST(Stmt, require_row__now_row__exception_thrown)
{
	SQLighter sql { setup_db("test_mock_n.db") };
	
	try
	{
		Stmt stmt = sql.execute(
			"CREATE TABLE test_mock ("
				"c_null TEXT NULL,"
				"c_value TEXT NULL"
			")");
		
		stmt.require_row();
		
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_NO_ROWS, e.code());
	}
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

TEST(Stmt, column_T)
{
	SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", 2, "c_dlb", 1.1, "c_str", "hello world") };
	auto stmt = stmt__select_mock(sql);
	
	ASSERT_EQ(true,				stmt.column<bool>(1));
	ASSERT_EQ(2,				stmt.column<int>(1));
	ASSERT_EQ(2,				stmt.column<int64_t>(1));
	ASSERT_EQ(1.1,				stmt.column<double>(2));
	ASSERT_EQ(1.1f,				stmt.column<float>(2));
	ASSERT_EQ("hello world",	stmt.column<std::string>(3));
}

TEST(Stmt, column_n_T)
{
	SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", 2, "c_dlb", 1.1, "c_str", "hello world") };
	auto stmt = stmt__select_mock(sql);
	
	bool		b;
	int			i;
	int64_t		i64;
	double		d;
	float		f;
	std::string s;
	
	ASSERT_TRUE(stmt.column_n<bool>(1, b));
	ASSERT_TRUE(stmt.column_n<int>(1, i));
	ASSERT_TRUE(stmt.column_n<int64_t>(1, i64));
	ASSERT_TRUE(stmt.column_n<double>(2, d));
	ASSERT_TRUE(stmt.column_n<float>(2, f));
	ASSERT_TRUE(stmt.column_n<std::string>(3, s));
	
	ASSERT_EQ(true,				b);
	ASSERT_EQ(2,				i);
	ASSERT_EQ(2,				i64);
	ASSERT_EQ(1.1,				d);
	ASSERT_EQ(1.1f,				f);
	ASSERT_EQ("hello world",	s);
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