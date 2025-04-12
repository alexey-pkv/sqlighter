#ifndef SQLIGHTER_CORE_DB_H
#define SQLIGHTER_CORE_DB_H


#include <string>
#include <filesystem>


struct sqlite3;


namespace sqlighter
{
	class DB
	{
	private:
		std::string m_path		{ };
		sqlite3*	m_db		{ nullptr };
		bool		m_closed	{ false };
		
		
	public:
		~DB();
		DB() = default;
		
		explicit DB(std::string path);
		explicit DB(const char* path);
		explicit DB(const std::filesystem::path& path);
		
		DB(const DB&) = delete;
		DB(DB&&) = delete;
		DB& operator=(const DB&) = delete;
		DB& operator=(DB&&) = delete;
		
		
	public:
		[[nodiscard]] inline const std::string& path() const noexcept { return m_path; }
		[[nodiscard]] inline const sqlite3* db() const noexcept { return m_db; }
		[[nodiscard]] inline sqlite3* db() noexcept { return m_db; }
		[[nodiscard]] inline bool is_open() const noexcept { return m_db != nullptr; }
		
		
	public:
		void close();
		void open();
		void open(std::string_view path);
		
		[[nodiscard]] const char* error_message() const;
	};
}


#endif
