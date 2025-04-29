#pragma once
#include <stack>
#include <functional>

namespace core {
	class AtExit
	{
		friend class EngineMain;
	public:
		static void add_func(const std::function<void()>& _Func) { mAtExitFuncs.push(_Func); }

	private:
		static void CallAtExit();
		static std::stack<std::function<void()>> mAtExitFuncs;

	private:
		AtExit() = delete;
		~AtExit() = delete;
	};
}

