#ifndef SQLIGHTER_CONNECTORS_CMDCREATETABLE_H
#define SQLIGHTER_CONNECTORS_CMDCREATETABLE_H


#include "base/connectors/CMD.h"
#include "base/connection/IConnection.h"


namespace sqlighter
{
	class CMDCreateTable : public CMD
	{
	private:
		bool		m_isTemp		{ false };
		bool		m_ifNotExists	{ false };
		std::string	m_scheme		{};
		std::string	m_table			{};
		
		std::vector<std::string>	m_columns	{};
		
		bool		m_withoutRowid	{ false };
		bool		m_strict		{ false };
		
	public:
		CMDCreateTable(CMDCreateTable&&) noexcept = default;
		CMDCreateTable(const CMDCreateTable&) = default;
		CMDCreateTable& operator=(const CMDCreateTable&) = default;
		CMDCreateTable& operator=(CMDCreateTable&&) = default;
		
		CMDCreateTable() = delete;
		
		explicit CMDCreateTable(const std::shared_ptr<IConnection>& connection);
		
		
	public:
		CMDCreateTable& temp();
		CMDCreateTable& if_not_exists();
		CMDCreateTable& scheme(std::string_view name);
		CMDCreateTable& table(std::string_view name);
		CMDCreateTable& table(std::string_view scheme, std::string_view table);
		
		CMDCreateTable& column_exp(std::string_view exp);
		
		CMDCreateTable& without_rowid();
		CMDCreateTable& strict();
		
		
	public:
		inline void create() const { execute().require_done(); }
		
		
	public: // CMD
		void assemble(std::ostringstream& ss) const override;
		[[nodiscard]] std::string assemble() const override;
		[[nodiscard]] std::vector<BindValue> bind() const override;
	};
}


#endif
