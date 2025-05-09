#ifndef SQLIGHTER_STMTREF_H
#define SQLIGHTER_STMTREF_H


#include "stmt_ptr.h"


namespace sqlighter
{
	class StmtRef
	{
	private:
		weak_stmt_ptr 	m_weak	{ };
		shared_stmt_ptr	m_stmt	{nullptr };
		
		
	public:
		StmtRef() = default;
		~StmtRef() = default;
		
		StmtRef(StmtRef&&) = default;
		StmtRef& operator=(StmtRef&&) = default;
		
		StmtRef(const StmtRef&) = delete;
		StmtRef& operator=(const StmtRef&) = delete;
		
		explicit StmtRef(weak_stmt_ptr weak);
		explicit StmtRef(shared_stmt_ptr m_stmt);
		explicit StmtRef(sqlite3_stmt* value);
		
		
	public:
		sqlite3_stmt* get();
		const sqlite3_stmt* get() const noexcept;
		
		void clear();
		
		
	public:
		inline sqlite3_stmt* get_cc() const noexcept { return const_cast<sqlite3_stmt*>(get()); }
		inline StmtRef weak() const noexcept { return StmtRef(m_stmt ? weak_stmt_ptr(m_stmt) : m_weak); }
		
		
	public:
		explicit operator bool() const;
	};
}


#endif
