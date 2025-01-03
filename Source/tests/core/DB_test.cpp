#include "core/DB.h"


#include <fstream>
#include <sqlite3.h>
#include <filesystem>

#include <gtest/gtest.h>


namespace fs = std::filesystem;


using namespace sqlighter;


fs::path get_db_directory() 
{
	return fs::path(__FILE__).parent_path() / ".db";
}

std::string get_db_file(std::string_view name) 
{
	return { get_db_directory() / name };
}

void create_db_folder() 
{
	fs::path db_folder = get_db_directory();
	
	if (!fs::exists(db_folder))
	{
		fs::create_directory(db_folder);
	}
}

void cleanup_db_folder()
{
	fs::path db_folder = get_db_directory();
	
	if (fs::exists(db_folder) && fs::is_directory(db_folder))
	{
		fs::remove_all(db_folder);
	}
}

fs::path setup()
{
	cleanup_db_folder();
	create_db_folder();
	
	return get_db_directory();
}

bool file_exists_in_db(const std::string_view filename)
{
	return fs::exists(get_db_directory() / filename);
}


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
	auto path = setup() / "open.test.db";
	
	DB db(path.c_str());
	
	
	db.open();
	
	
	ASSERT_TRUE(file_exists_in_db("open.test.db"));
	ASSERT_TRUE(db.is_open());
	ASSERT_EQ(path.c_str(), db.path());
}

TEST(DB, open__with_path)
{
	auto path = setup() / "open.test.db";
	auto otherPath = get_db_file("diff.name.db");
	
	DB db(path.c_str());
	
	
	db.open(otherPath);
	
	
	ASSERT_FALSE(file_exists_in_db("open.test.db"));
	ASSERT_TRUE(file_exists_in_db("diff.name.db"));
	ASSERT_TRUE(db.is_open());
	ASSERT_EQ(otherPath.c_str(), db.path());
}

TEST(DB, close__opened_connection_closed)
{
	auto path = setup() / "open.test.db";
	
	DB db(path.c_str());
	
	db.open();
	
	
	db.close();
	
	
	ASSERT_TRUE(file_exists_in_db("open.test.db"));
	ASSERT_FALSE(db.is_open());
}

TEST(DB, db)
{
	auto path = setup() / "open.test.db";
	
	DB db(path.c_str());
	
	db.open();
	
	
	const auto* a = db.db();
	auto* b = db.db();
	
	
	ASSERT_EQ(a, b);
	ASSERT_NE(a, nullptr);
}