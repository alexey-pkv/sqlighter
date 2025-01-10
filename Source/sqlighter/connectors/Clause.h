#pragma once


#include <vector>
#include <sstream>

#include <span>

#include "core/BindValue.h"
#include "query_utils.h"


class sqlite3_stmt;


namespace sqlighter
{
	class Clause
	{
	private:
		const char*				m_delimiter = nullptr;
		bool					m_isEmpty	= true;
		std::stringstream		m_stream;
		std::vector<BindValue>	m_binds;
		
		
	private:
		void append_delimiter();
		
		
	public:
		Clause() = delete;
		~Clause() = default;
		
		explicit Clause(const char* delimiter);
		
		Clause(const Clause&);
		Clause& operator=(const Clause&);
		Clause(Clause&&) noexcept = default;
		Clause& operator=(Clause&&) noexcept = default;
		
		
	public:
		[[nodiscard]] inline bool empty_clause() const noexcept { return m_isEmpty; }
		[[nodiscard]] inline bool has_binds() const noexcept { return !m_binds.empty(); }
		[[nodiscard]] inline std::string get_clause_string() const { return m_stream.str(); }
		
		[[nodiscard]] inline const std::vector<BindValue>& binds() const noexcept { return m_binds; }
		[[nodiscard]] inline const std::stringstream& clause() const noexcept { return m_stream; }
		[[nodiscard]] inline size_t binds_size() const noexcept { return m_binds.size(); }
		
		
	public:
		void append(std::string_view str);
		void append_bind(const BindValue& bind);
		void append_binds(std::span<const BindValue> binds);
		void append_binds(std::initializer_list<BindValue> binds);
		void append_directly(std::string_view str);
		void append(std::string_view str, std::span<const BindValue> binds);
		void append_directly(std::string_view str, std::initializer_list<BindValue> binds);
		void append_directly(std::string_view str, std::span<const BindValue> binds);
		
		int bind(sqlite3_stmt* to, int offset) const;
		
		void append_to(std::stringstream& to) const;
		void append_to(std::vector<BindValue>& to) const;
		
		
	public:
		inline std::stringstream& next_section()
		{
			append_delimiter();
			return m_stream;
		}
		
		inline std::stringstream& direct()
		{
			return m_stream;
		}
		
		inline std::stringstream& operator<<(const col& c)
		{
			auto& ss = next_section();
			return ss << c;
		}
		
		inline std::stringstream& operator<<(const col_as& c)
		{
			return next_section() << c;
		}
	};
}
