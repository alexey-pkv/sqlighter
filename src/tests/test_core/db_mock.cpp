#include "db_mock.h"


#include "sqlighter.h"
#include "connection/Connection.h"
#include "exceptions/sqlighter_exceptions.h"


namespace fs = std::filesystem;


using namespace sqlighter;


fs::path get_db_directory() 
{
	return fs::path(__FILE__).parent_path() / ".db";
}

std::string get_db_file(std::string_view name) 
{
	return (get_db_directory() / name).u8string();
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

fs::path setup_db_dir()
{
	cleanup_db_folder();
	create_db_folder();
	
	return get_db_directory();
}

fs::path setup_db(std::string_view db_name)
{
	return setup_db_dir() / db_name;
}

bool is_file_exists_in_db_dir(const std::string_view filename)
{
	return fs::exists(get_db_directory() / filename);
}

SQLighter create_mock_table(
	std::string_view name, 
	std::string_view null_column_name	, 
	std::string_view int_column_name	, int64_t int_value,
	std::string_view dlb_column_name	, double dbl_value,
	std::string_view txt_column_name	, std::string_view text_value)
{
	SQLighter sql(setup_db("mock_db.db"));
	
	{
		auto d_create= sql.direct();
		
		d_create << 
			"CREATE TABLE " << name << " ("
			"	" << null_column_name	<< " TEXT NULL,"
			"	" << int_column_name	<< " INTEGER NOT NULL,"
			"	" << dlb_column_name	<< " REAL NOT NULL,"
			"	" << txt_column_name	<< " TEXT NOT NULL"
			")";
		
		d_create.execute_direct();
	}
	
	{
		auto d_insert= sql.direct();
		
		d_insert << 
			"INSERT INTO " << name << " ("
				<< null_column_name	<< ","
				<< int_column_name	<< ","
				<< dlb_column_name	<< ","
				<< txt_column_name	<<  
			")"
			"VALUES (NULL, ?, ?, ?)";
		
		d_insert.append({ int_value, dbl_value, text_value });
		d_insert.execute_direct();
	}
	
	return sql;
}

