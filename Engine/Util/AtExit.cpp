#include "AtExit.h"

namespace core {
	std::stack<std::function<void()>> AtExit::m_atExitFuncs;

	void AtExit::on_exit()
	{
		while (false == m_atExitFuncs.empty())
		{
			m_atExitFuncs.top()();
			m_atExitFuncs.pop();
		}
	}
}


