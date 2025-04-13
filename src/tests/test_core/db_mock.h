#pragma once


#include <filesystem>

#include "base/connection/IConnection.h"


namespace sqlighter
{
	class SQLighter;
}


std::filesystem::path get_db_directory();
std::string get_db_file(std::string_view name);
void create_db_folder();
void cleanup_db_folder();
std::filesystem::path setup_db_dir();
std::filesystem::path setup_db(std::string_view db_name);
bool is_file_exists_in_db_dir(const std::string_view filename);

sqlighter::SQLighter create_mock_table(
	std::string_view name, 
	std::string_view null_column_name	= "c_null", 
	std::string_view int_column_name	= "c_int", int64_t int_value			= 15,
	std::string_view dlb_column_name	= "c_dbl", double dbl_value				= 16.16,
	std::string_view txt_column_name	= "c_txt", std::string_view text_value	= "Hello World");