#ifndef SQLIGHTER_BINDVALUE_H
#define SQLIGHTER_BINDVALUE_H


#include <vector>
#include <string>
#include <cstdint>


class sqlite3_stmt;


namespace sqlighter
{
	class BindValue
	{
	public:
		enum class type
		{
			INT_32		= 0,
			INT_64		= 1,
			DOUBLE		= 2,
			NULL_VAL	= 3,
			TEXT		= 4,
			TEXT_16		= 5,
			TEXT_64		= 6,
		};
		
		
	public:
		union value
		{
			int32_t		i32;
			int64_t		i64;
			double		dbl;
		};
		
		
	private:
		type		m_type;
		value		m_value;
		std::string m_strValue;
		
		
	public:
		~BindValue() = default;
		BindValue(BindValue&&) = default;
		BindValue(const BindValue&) = default;
		BindValue& operator=(BindValue&&) = default;
		BindValue& operator=(const BindValue&) = default;
		
		BindValue();
		explicit BindValue(type t);
		
		BindValue(int32_t val);				// NOLINT(*-explicit-constructor)
		BindValue(int64_t val);				// NOLINT(*-explicit-constructor)
		BindValue(double val);				// NOLINT(*-explicit-constructor)
		BindValue(std::string_view val);	// NOLINT(*-explicit-constructor)
		BindValue(const char* val);			// NOLINT(*-explicit-constructor)		
		
	public:
		[[nodiscard]] inline value get_value() const noexcept					{ return m_value; }
		[[nodiscard]] inline const std::string& get_str_value() const noexcept	{ return m_strValue; }
		[[nodiscard]] inline type get_type() const noexcept						{ return m_type; }
		
		inline void set_value(value v) noexcept					{ m_value = v; }
		inline void set_str_value(const std::string_view & str)	{ m_strValue = str; }
		inline void set_type(type t) noexcept					{ m_type = t; }
		
		
	public:
		void bind(sqlite3_stmt* stmt, int offset) const;
		void to_error_message(std::ostringstream& stream) const;
	};
	
	
	template <typename T, typename = std::enable_if_t<std::is_constructible<BindValue, T>::value>>
	inline BindValue bind(const T& t) { return BindValue(t); }
	
	inline auto bind(std::initializer_list<BindValue> values) { return std::vector<BindValue> { values }; }
}


#endif
