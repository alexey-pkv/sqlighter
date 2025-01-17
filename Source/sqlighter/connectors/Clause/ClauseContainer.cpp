#include "ClauseContainer.h"


using namespace sqlighter;


ClauseContainer::ClauseContainer(const char* c) : m_clause(c) {}


void ClauseContainer::append(std::string_view exp, const std::vector<BindValue>& bind)
{
	m_clause.append(exp, bind);
}


std::ostream& sqlighter::operator<<(std::ostream& ss, const ClauseContainer& ct)
{
	ct.append_to(ss);
	return ss;
}