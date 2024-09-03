#pragma once
#include "Engine/Util/LazySingleton.h"

#include "Engine/Entity.h"

namespace ehw {
	/*
	* LazySingleton을 사용해야 하는 이유
	static 변수 초기화 시점에 값이 생성됨
	static 변수는 기본적인 변수들은 0, nullptr 등으로 초기화 되지만
	static 타입의 stl 컨테이너는 초기화되지 않아 사용이 불가능하다...
	InstanceManager 안의 static 컨테이너도 마찬가지로 static을 선언할 시 사용이 불가능하다.
	대신 힙에 생성하는 Lazy Singleton 클래스를 통해서 초기화가 가능
	마찬가지로 AtExit도 사용 불가능 -> 직접 메모리 해제를 해주어야 함
	*/
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
			m_ctors[_name] = 
				[]()->Entity* {
				return static_cast<Entity*>(new T);
				};
		}

	private:
		std::unordered_map<std::string_view, std::function<Entity* ()>> m_ctors;
	};
}


