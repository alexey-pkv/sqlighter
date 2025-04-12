#ifndef SQLIGHTER_CORE_SCALARVALUE_H
#define SQLIGHTER_CORE_SCALARVALUE_H


#include <string>

#include "blob.h"


namespace sqlighter
{
	class ScalarValue
	{	
	public:
		enum class type
		{
			INT			= 0,
			DOUBLE		= 1,
			NULL_VAL	= 2,
			TEXT		= 3,
			BLOB		= 4
		};
	
	
	private:
		type		m_type	{ type::NULL_VAL };
		int64_t		m_i64	{};
		double		m_dbl	{};
		std::string	m_str	{};
		blob_t		m_blob	{};
		
		
	public:
		ScalarValue() = default;
		
		explicit ScalarValue(ScalarValue::type t);
		
		ScalarValue(int32_t val);				// NOLINT(*-explicit-constructor)
		ScalarValue(int64_t val);				// NOLINT(*-explicit-constructor)
		ScalarValue(double val);				// NOLINT(*-explicit-constructor)
		ScalarValue(std::string_view val);		// NOLINT(*-explicit-constructor)
		ScalarValue(const char* val);			// NOLINT(*-explicit-constructor)
		ScalarValue(size_t size, void* blob);	// NOLINT(*-explicit-constructor)
		ScalarValue(blob_t blob);				// NOLINT(*-explicit-constructor)
		
		
	public:
		inline type get_type() const noexcept { return m_type; }
		inline bool is_null() const noexcept { return m_type == type::NULL_VAL; }
		
		
	public:
		int32_t get_int32(int32_t def = 0) const;
		int64_t get_int64(int64_t def = 0) const;
		double get_double(double def = 0) const;
		const std::string& get_str() const;
		const blob_t& get_blob() const;
		
		bool try_get_int32(int32_t& v) const noexcept;
		bool try_get_int64(int64_t& v) const noexcept;
		bool try_get_double(double & v) const noexcept;
		bool try_get_str(std::string& v) const noexcept;
		bool try_get_blob(blob_t& v) const noexcept;
		
		
	public:
		template <typename T> inline T get() const = delete;
		template <typename T> inline T get(T def) const = delete;
		template <typename T> inline bool try_get(T& t) const = delete;
		
		
	public:
		static inline ScalarValue null() noexcept { return ScalarValue(type::NULL_VAL); };
	};
	
	template<> inline bool ScalarValue::get() const						{ return get_int64(0) != 0; }
	template<> inline int32_t ScalarValue::get() const					{ return get_int32(0); }
	template<> inline int64_t ScalarValue::get() const					{ return get_int64(0); }
	template<> inline double ScalarValue::get() const					{ return get_double(0); }
	template<> inline int32_t ScalarValue::get(int32_t def) const		{ return get_int32(def); }
	template<> inline int64_t ScalarValue::get(int64_t def) const		{ return get_int64(def); }
	template<> inline double ScalarValue::get(double def) const			{ return get_double(def); }
	template<> inline const std::string& ScalarValue::get() const		{ return get_str(); }
	template<> inline const blob_t& ScalarValue::get() const			{ return get_blob(); }
	
	template<> inline bool ScalarValue::try_get(int32_t& v) const		{ return try_get_int32(v); }
	template<> inline bool ScalarValue::try_get(int64_t& v) const		{ return try_get_int64(v); }
	template<> inline bool ScalarValue::try_get(double& v) const		{ return try_get_double(v); }
	template<> inline bool ScalarValue::try_get(std::string& v) const	{ return try_get_str(v); }
	template<> inline bool ScalarValue::try_get(blob_t& v) const		{ return try_get_blob(v); }
}


#endif
