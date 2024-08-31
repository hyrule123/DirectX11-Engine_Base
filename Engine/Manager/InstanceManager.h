#pragma once
#include "Engine/Util/StaticSingleton.h"

#include "Engine/Entity.h"

namespace ehw {
	class InstanceManager
		: public StaticSingleton<InstanceManager>
	{
		friend class GameEngine;
		friend class StaticSingleton<InstanceManager>;
	private:
		InstanceManager();
		~InstanceManager();

		void init();
		void release();

	public:
		Entity* Instantiate(const std::string_view key);

		template <typename T>
		void add_ctor(const std::string_view _name) {
			m_ctors.insert(
				std::make_pair(_name, 
					[]()->Entity* {
						return static_cast<Entity*>(new T);
					}
					)
			);
		}

	private:
		std::unordered_map<std::string_view, std::function<Entity* ()>> m_ctors;
	};
}


