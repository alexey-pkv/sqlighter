#include <gtest/gtest.h>


#include "sqlighter.h"
#include "db_mock.h"


void print_versions()
{
	sqlighter::SQLighter sql { setup_db("test_select.db") };
	
	
	std::cout << "Running with:" << std::endl;
	std::cout << "    SQLite     : " << sql.sqlight_version() << std::endl;
	std::cout << "    SQLite Lib : " << sqlighter::SQLighter::sqlight_lib_version() << std::endl;
	std::cout << "==========================================" << std::endl;
}


int main(int argc, char **argv)
{
	print_versions();
	
	
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}