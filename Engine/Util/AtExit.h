#pragma once
#include <stack>
#include <functional>

namespace core {
	class AtExit
	{
		friend class EngineMain;
	public:
		static void add_func(const std::function<void()>& _Func) { m_atExitFuncs.push(_Func); }

	private:
		static void on_exit();
		static std::stack<std::function<void()>> m_atExitFuncs;

	private:
		AtExit() = delete;
		~AtExit() = delete;
	};
}

