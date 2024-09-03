#pragma once
#include "Engine/Util/LazySingleton.h"

#include "Engine/Entity.h"

namespace ehw {
	class InstanceManager
		: public LazySingleton<InstanceManager>
	{
		friend class GameEngine;
		friend class LazySingleton<InstanceManager>;
	private:
		InstanceManager();
		~InstanceManager();

	public:
		Entity* Instantiate(const std::string_view key);

		template <typename T>
		T* Instantiate(const std::string_view key) {
			Entity* e = Instantiate(key);
			T* ret = nullptr;
			ret = dynamic_cast<T*>(e);

			if (nullptr == ret) {
				SAFE_DELETE(e);
			}

			return ret;
		}

		template <typename T> requires std::is_base_of_v<Entity, T>
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


