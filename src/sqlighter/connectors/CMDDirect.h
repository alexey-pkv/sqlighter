#ifndef SQLIGHTER_CONNECTORS_CMDDIRECT_H
#define SQLIGHTER_CONNECTORS_CMDDIRECT_H


#include <sstream>

#include "core/BindValue.h"
#include "base/connectors/ICMD.h"
#include "base/connection/IConnection.h"


namespace sqlighter
{
	class CMDDirect : public CMD
	{
	private:
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
		inline CMDDirect& append(std::string_view exp)											{ return do_append(exp, {}); };			\
		inline CMDDirect& append(std::string_view exp, BindValue value)							{ return do_append(exp, { value }); };		\
		inline CMDDirect& append(std::string_view exp, std::initializer_list<BindValue> values)	{ return do_append(exp, { values }); };	\
		inline CMDDirect& append(std::string_view exp, const std::vector<BindValue>& values)	{ return do_append(exp, values); }
		
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
	};
}


#endif
