#pragma once
#include <stack>
#include <functional>

namespace core {
	class AtExit
	{
	private:
		static std::stack<std::function<void()>> mAtExitFuncs;
	public:
		static void AddFunc(std::function<void()> _Func) { mAtExitFuncs.push(_Func); }
		static void CallAtExit();

	private:
		AtExit() = delete;
		~AtExit() = delete;
	};
}

