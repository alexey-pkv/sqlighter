#ifndef SQLIGHTER_STMTREF_H
#define SQLIGHTER_STMTREF_H


#include "unique_stmt_ptr.h"


namespace sqlighter
{
	class StmtRef
	{
	private:
		unique_stmt_ptr				m_unique	{ nullptr };
		std::weak_ptr<sqlite3_stmt>	m_weak		{};	
		
		
	public:
		StmtRef() = default;
		~StmtRef() = default;
		
		StmtRef(StmtRef&&) = default;
		StmtRef& operator=(StmtRef&&) = default;
		
		StmtRef(const StmtRef&) = delete;
		StmtRef& operator=(const StmtRef&) = delete;
		
		explicit StmtRef(sqlite3_stmt* value);
		
		
	public:
		sqlite3_stmt* get();
		const sqlite3_stmt* get() const noexcept;
		void set(sqlite3_stmt* value);
		void set(std::shared_ptr<sqlite3_stmt> value);
		void clear();
		
		
	public:
		inline sqlite3_stmt* get_cc() const noexcept { return const_cast<sqlite3_stmt*>(get()); }
		
		
	public:
		explicit operator bool() const;
	};
}


#endif
