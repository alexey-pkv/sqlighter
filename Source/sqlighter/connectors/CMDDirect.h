#pragma once


#include <sstream>

#include "base/connectors/ICMD.h"
#include "base/connection/IConnection.h"


namespace sqlighter
{
	class CMDDirect : public ICMD
	{
	private:
		std::shared_ptr<IConnection>	m_connection;
		
		std::ostringstream		m_stream;
		std::vector<BindValue>	m_binds;
		
		
	public:
		CMDDirect(CMDDirect&&) noexcept = default;
		CMDDirect& operator=(CMDDirect&&) = default;
		
		CMDDirect(const CMDDirect&);
		CMDDirect& operator=(const CMDDirect&);
		
		CMDDirect() = delete;
		
		explicit CMDDirect(const std::shared_ptr<IConnection>& connection);
		
		
	protected:
		CMDDirect& do_append(std::string_view exp, const std::vector<BindValue>& span);
		
		
	public:
		SQLIGHTER_INLINE_CLAUSE(append, do_append, CMDDirect);
		
		inline CMDDirect& append(std::initializer_list<BindValue> values) { return do_append({}, { values }); };
		inline CMDDirect& operator<<(std::string_view exp) { do_append(exp, {}); return *this; }
		inline CMDDirect& operator<<(const std::vector<BindValue>& binds) { do_append({}, binds); return *this; }
		
		template<class T, typename = std::enable_if_t<std::is_same<T, BindValue>::value>>
		inline CMDDirect& operator<<(T bind) { do_append({}, { bind }); return *this; }
		
		
	public:
		void execute_direct() const;
		
		
	public:
		void assemble(std::ostringstream& ss) const override;
		
		[[nodiscard]] std::string assemble() const override;
		[[nodiscard]] std::vector<BindValue> bind() const override;
		
		Stmt execute() const override;
	};
}