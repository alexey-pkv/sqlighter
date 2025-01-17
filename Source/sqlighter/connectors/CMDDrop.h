#pragma once


#include "base/connectors/ICMD.h"
#include "base/connection/IConnection.h"


namespace sqlighter
{
	class CMDDrop : public CMD
	{
	private:
		bool		m_ifExists	{ false };
		std::string m_element	{ };
		std::string m_scheme	{ };
		std::string m_name		{ };
		
		
	private:
		CMDDrop& element(std::string_view e, std::string_view name);
		CMDDrop& element(std::string_view e, std::string_view scheme, std::string_view name);
		
		
	public:
		CMDDrop() = delete;
		CMDDrop(CMDDrop&&) noexcept = default;
		CMDDrop(const CMDDrop&) noexcept = default;
		CMDDrop& operator=(CMDDrop&&) = default;
		CMDDrop& operator=(const CMDDrop&) = default;
		
		explicit CMDDrop(const std::shared_ptr<IConnection>& connection);
		
		
	public:
		CMDDrop& if_exists();
		
		inline CMDDrop& table(std::string_view name) { return element("TABLE", name); }
		inline CMDDrop& table(std::string_view scheme, std::string_view name) { return element("TABLE", scheme, name); }
		inline CMDDrop& view(std::string_view name) { return element("VIEW", name); }
		inline CMDDrop& view(std::string_view scheme, std::string_view name) { return element("VIEW", scheme, name); }
		inline CMDDrop& trigger(std::string_view name) { return element("TRIGGER", name); }
		inline CMDDrop& trigger(std::string_view scheme, std::string_view name) { return element("TRIGGER", scheme, name); }
		inline CMDDrop& index(std::string_view name) { return element("INDEX", name); }
		inline CMDDrop& index(std::string_view scheme, std::string_view name) { return element("INDEX", scheme, name); }
		
		
	public:
		[[nodiscard]] std::string assemble() const override;
		void assemble(std::ostringstream& ss) const override;
		[[nodiscard]] std::vector<BindValue> bind() const override;
	};
}

