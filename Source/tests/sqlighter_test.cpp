#include "sqlighter.h"


#include "db_mock.h"
#include "core/DB.h"

#include <regex>
#include <gtest/gtest.h>


using namespace sqlighter;


TEST(sqlighter, transaction)
{
	SQLighter sql { setup_db("test_select.db") };
	
	sql.execute(
		"CREATE TABLE ExampleTable (                    "
		"	Name			TEXT NOT NULL PRIMARY KEY,	"
		"	Age				INTEGER						"
		")"
	);
	
	sql.execute(
		"INSERT INTO ExampleTable (Name, Age) VALUES "
		"	('Alice',	30),   	"
		"	('Bob',		NULL)	"
	);
	
	
	sql.begin();
	sql.execute("UPDATE ExampleTable SET Age = ? WHERE Age IS NULL", 20);
	sql.commit();
	
	ASSERT_EQ(1, sql.select().from("ExampleTable").by_field("Age", 20).query_count());
	
	
	sql.begin();
	sql.execute("UPDATE ExampleTable SET Age = ? WHERE Age = 30", 40);
	ASSERT_EQ(1, sql.select().from("ExampleTable").by_field("Age", 40).query_count());
	ASSERT_EQ(0, sql.select().from("ExampleTable").by_field("Age", 30).query_count());
	sql.rollback();
	
	ASSERT_EQ(1, sql.select().from("ExampleTable").by_field("Age", 30).query_count());
	ASSERT_EQ(0, sql.select().from("ExampleTable").by_field("Age", 40).query_count());
}

TEST(sqlighter, path)
{
	SQLighter sql { "hello world" };
	
	
	ASSERT_EQ("hello world", sql.path());
}

TEST(sqlighter, constructor__using_db)
{
	auto db = std::make_shared<DB>("some path");
	SQLighter sql { db };
	
	
	ASSERT_EQ("some path", sql.path());
	ASSERT_EQ(db, sql.db());
}

TEST(sqlighter, sanity__reindex)
{
	SQLighter sql { setup_db("test_select.db") };
	
	sql.execute("CREATE TABLE Users (UserID INTEGER PRIMARY KEY AUTOINCREMENT)");
	sql.execute("CREATE INDEX idx_userid ON Users(UserID);");
	
	
	sql.reindex("Users");
	sql.reindex("main.Users");
	sql.reindex("main", "Users");
}


TEST(sqlighter, sanity__getters)
{
	SQLighter sql { setup_db("test_select.db") };
	
	sql.direct();
	sql.create();
	sql.drop();
	
	sql.select();
	sql.insert();
	sql.update();
	sql.del();
}


TEST(sqlighter, sqlite_version)
{
	SQLighter sql { setup_db("test_select.db") };
	std::regex versionRegex(R"(\d+\.\d+\.\d+)");
	
	
	auto version = sql.sqlite_version();
	
	
	ASSERT_TRUE(std::regex_match(version, versionRegex));
}


TEST(sqlighter, sqlight_lib_version)
{
	std::regex versionRegex(R"(\d+\.\d+\.\d+)");
	
	
	auto version = sqlighter::sqlite_lib_version();
	
	
	ASSERT_TRUE(std::regex_match(version, versionRegex));
}


TEST(sqlighter, sqlighter_lib_version)
{
	std::regex versionRegex(R"(\d+\.\d+\.\d+)");
	
	
	auto version = sqlighter::sqlighter_lib_version();
	
	
	ASSERT_TRUE(std::regex_match(version, versionRegex));
}


TEST(sqlighter, memory)
{
	auto s = SQLighter::memory();
	
	
	ASSERT_EQ(":memory:", s.path());
}