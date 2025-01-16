#pragma once


#include "base/connectors/ICMD.h"
#include "base/connection/IConnection.h"


namespace sqlighter
{
	class CMDCreateTable : public ICMD
	{
	private:
		std::shared_ptr<IConnection>	m_connection;
		
		
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
		
		
	public: // ICMD
		void assemble(std::ostringstream& ss) const override;
		[[nodiscard]] std::string assemble() const override;
		[[nodiscard]] std::vector<BindValue> bind() const override;
		
		Stmt execute() const override;
	};
}

