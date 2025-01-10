#include "core/DB.h"


#include <fstream>
#include <gtest/gtest.h>

#include "../test_core/db_mock.h"


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
	
	DB db(path.c_str());
	
	
	db.open();
	
	
	ASSERT_TRUE(is_file_exists_in_db_dir("open.test.db"));
	ASSERT_TRUE(db.is_open());
	ASSERT_EQ(path.c_str(), db.path());
}

TEST(DB, open__with_path)
{
	auto path = setup_db_dir() / "open.test.db";
	auto otherPath = get_db_file("diff.name.db");
	
	DB db(path.c_str());
	
	
	db.open(otherPath);
	
	
	ASSERT_FALSE(is_file_exists_in_db_dir("open.test.db"));
	ASSERT_TRUE(is_file_exists_in_db_dir("diff.name.db"));
	ASSERT_TRUE(db.is_open());
	ASSERT_EQ(otherPath.c_str(), db.path());
}

TEST(DB, close__opened_connection_closed)
{
	auto path = setup_db_dir() / "open.test.db";
	
	DB db(path.c_str());
	
	db.open();
	
	
	db.close();
	
	
	ASSERT_TRUE(is_file_exists_in_db_dir("open.test.db"));
	ASSERT_FALSE(db.is_open());
}

TEST(DB, db)
{
	auto path = setup_db_dir() / "open.test.db";
	
	DB db(path.c_str());
	
	db.open();
	
	
	const auto* a = db.db();
	auto* b = db.db();
	
	
	ASSERT_EQ(a, b);
	ASSERT_NE(a, nullptr);
}