#pragma once


#include <vector>
#include <sstream>

#include "core/BindValue.h"
#include "connectors/query_utils.h"


class sqlite3_stmt;


namespace sqlighter
{
	class Clause
	{
	private:
		const char*				m_delimiter = nullptr;
		bool					m_isEmpty	= true;
		std::ostringstream		m_stream;
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
		[[nodiscard]] inline bool is_empty_clause() const noexcept { return m_isEmpty; }
		[[nodiscard]] inline bool has_binds() const noexcept { return !m_binds.empty(); }
		[[nodiscard]] inline std::string get_clause_string() const { return m_stream.str(); }
		
		[[nodiscard]] inline const std::vector<BindValue>& binds() const noexcept { return m_binds; }
		[[nodiscard]] inline const std::ostringstream& clause() const noexcept { return m_stream; }
		[[nodiscard]] inline size_t binds_size() const noexcept { return m_binds.size(); }
		
		
	public:
		void append(std::string_view str);
		void append_bind(const BindValue& bind);
		void append_binds(const std::vector<BindValue>& binds);
		void append_binds(std::initializer_list<BindValue> binds);
		void append_directly(std::string_view str);
		void append(std::string_view str, const std::vector<BindValue>& binds);
		void append_directly(std::string_view str, std::initializer_list<BindValue> binds);
		void append_directly(std::string_view str, const std::vector<BindValue>& binds);
		
		void append_to(std::ostream& to) const;
		void append_to(std::vector<BindValue>& to) const;
		
		void clear();
		
		
	public:
		inline std::ostringstream& next_section()
		{
			append_delimiter();
			return m_stream;
		}
		
		inline std::ostringstream& direct()
		{
			return m_stream;
		}
		
		inline std::ostringstream& operator<<(const col_as& c)
		{
			next_section() << c;
			return m_stream;
		}
	};
	
	
	inline std::ostream& operator<<(std::ostream& ss, const Clause& ct)
	{
		ct.append_to(ss);
		return ss;
	}
}
