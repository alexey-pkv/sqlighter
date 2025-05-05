#ifndef SQLIGHTER_BASE_CONNECTORS_PREPARED_H
#define SQLIGHTER_BASE_CONNECTORS_PREPARED_H


#include "ICMD.h"


namespace sqlighter
{
	class Prepared : public CMD
	{
	private:
		bool					m_autoReset		= false;
		bool					m_isSetup		= false;
		int 					m_bindsCount	= 0;
		
		std::string 			m_command;
		std::vector<BindValue>	m_binds;
		
		
	public:
		~Prepared() override = default; // LCOV_EXCL_LINE
		
		Prepared(
			const std::shared_ptr<IConnection>& connection,
			std::string&& command, 
			std::vector<BindValue>&& binds);
		
		
	public:
		[[nodiscard]] std::string assemble() const override;
		void assemble(std::ostringstream& ss) const override;
		[[nodiscard]] std::vector<BindValue> bind() const override;
		Stmt execute() const override;
		
	public:
		void set_auto_reset_binds(bool autoReset);
		void reset_binds();
	};
}


#endif
