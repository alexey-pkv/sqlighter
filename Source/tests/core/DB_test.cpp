#include "core/DB.h"


#include <fstream>
#include <gtest/gtest.h>
#include <filesystem>

#include "../test_core/db_mock.h"
#include "sqlighter.h"
#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


TEST(DB, constructor__new_object_without_path)
{
	DB db;
	
	
	ASSERT_FALSE(db.is_open());
	ASSERT_TRUE(db.path().empty());
	ASSERT_EQ(nullptr, db.db());
	
	// Validate no exception is thrown.
	db.close();
}

TEST(DB, constructor__new_object_with_path)
{
	DB db("some/path");
	
	
	ASSERT_FALSE(db.is_open());
	ASSERT_EQ("some/path", db.path());
	ASSERT_EQ(nullptr, db.db());
	
	// Validate no exception is thrown.
	db.close();
}

TEST(DB, open)
{
	auto path = setup_db_dir() / "open.test.db";
	
	DB db(path);
	
	
	db.open();
	
	
	ASSERT_TRUE(is_file_exists_in_db_dir("open.test.db"));
	ASSERT_TRUE(db.is_open());
	ASSERT_EQ(path, db.path());
}

TEST(DB, open__with_path)
{
	auto path = setup_db_dir() / "open.test.db";
	auto otherPath = get_db_file("diff.name.db");
	
	DB db(path);
	
	
	db.open(otherPath);
	
	
	ASSERT_FALSE(is_file_exists_in_db_dir("open.test.db"));
	ASSERT_TRUE(is_file_exists_in_db_dir("diff.name.db"));
	ASSERT_TRUE(db.is_open());
	ASSERT_EQ(otherPath.c_str(), db.path());
}

TEST(DB, close__opened_connection_closed)
{
	auto path = setup_db_dir() / "open.test.db";
	
	DB db(path);
	
	db.open();
	
	
	db.close();
	
	
	ASSERT_TRUE(is_file_exists_in_db_dir("open.test.db"));
	ASSERT_FALSE(db.is_open());
}

TEST(DB, db)
{
	auto path = setup_db_dir() / "open.test.db";
	
	DB db(path);
	
	db.open();
	
	
	const auto* a = db.db();
	auto* b = db.db();
	
	
	ASSERT_EQ(a, b);
	ASSERT_NE(a, nullptr);
}


TEST(DB, close__failed_to_close__exception_thrown)
{
	SQLighter sql { create_mock_table("mock_table") };
	
	auto stmt = sql.select().column_exp("1").execute();
	
	try
	{
		sql.db()->close();
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_FAILED_TO_CLOSE_DB, e.code());
	}
}

TEST(DB, open__db_opened)
{
	std::string path;
	
	{
		SQLighter sql { create_mock_table("mock_table", "c_null", "c_int", 1) };
		path = sql.path();
	}
	
	{
		SQLighter sql(path);
		
		ASSERT_EQ(1, sql.select().from("mock_table").query_count());
	}
}

TEST(DB, open__empty_path__error_thrown)
{
	SQLighter sql("");
	
	try
	{
		sql.open();
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_FAILED_TO_OPEN_DB, e.code());
	}
}


TEST(DB, open__failed_to_open__error_thrown)
{
	auto path = std::filesystem::path("var") / "non-existing-dir" / "example.db";
	
	SQLighter sql(path);
	
	try
	{
		sql.open();
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_FAILED_TO_OPEN_DB, e.code());
	}
}

TEST(DB, error_message__db_closed__return_empty_string)
{
	SQLighter sql { create_mock_table("db") };
	
	sql.db()->close();
	
	ASSERT_TRUE(std::string { sql.db()->error_message() }.empty());
}

TEST(DB, open__db_was_closed__error_thrown)
{
	SQLighter sql { create_mock_table("db") };
	
	sql.db()->close();
	
	try
	{
		sql.db()->open(setup_db("db_2").string());
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_CONNECTION_WAS_CLOSED, e.code());
	}
}

TEST(DB, open__db_open_on_another_path__error_thrown)
{
	SQLighter sql { create_mock_table("db") };
	
	try
	{
		sql.db()->open(sql.path() + ".2");
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_CONNECTION_ALREADY_OPEN, e.code());
	}
}

TEST(DB, open__db_open_on_same_path__no_error)
{
	SQLighter sql { create_mock_table("db") };
	
	
	ASSERT_TRUE(sql.db()->is_open());
	sql.db()->open(sql.path());
	ASSERT_TRUE(sql.db()->is_open());
}