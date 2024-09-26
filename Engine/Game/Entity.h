#pragma once
#include "Engine/Common.h"
#include "Engine/Util/LazySingleton.h"

#include <type_traits>

//클래스의 헤더 최상단에 작성. 클래스별 이름을 등록한다.
#define CLASS_NAME(_class_) \
public: static inline constexpr const std::string_view concrete_class_name = #_class_

//CLONE_ABLE 선언시 반드시 복사생성자를 재정의해줘야 함
#define CLONE_ABLE(_type) \
public: virtual _type* Clone() override { return new _type(*this); }

//Clone을 지원하지 않을 경우 nullptr이 반환된다.
#define CLONE_DISABLE(_type) \
private: virtual _type* Clone() override { return nullptr; }

#define REGISTER_INSTANCE(_class, _key) \
private: static inline const bool UNIQUE_VAR(unused) = EntityFactory::get_inst().add_ctor<_class>(_key)

#define REGISTER_INSTANCE_DEFAULT(_class) REGISTER_INSTANCE(_class, _class::concrete_class_name)

namespace ehw
{
	class Entity
		: public std::enable_shared_from_this<Entity>
	{
	public:
		Entity(const std::string_view _class_concrete_name);
		Entity(const Entity& _other);

		//Clone을 지원하지 않을 경우 nullptr이 반환된다.
		virtual Entity* Clone() { return nullptr; }

		virtual ~Entity();

		const std::string_view get_concrete_class_name() const {
			return m_concrete_class_name;
		}
		uint32 GetID() const { return m_ID; }

		template <typename T>
		std::shared_ptr<T> shared_from_this_cast() {
			return std::dynamic_pointer_cast<T>(shared_from_this());
		}

	private:
		const uint32 m_ID;
		const std::string_view m_concrete_class_name;

		static uint32 g_nextID;
	};

	//이 클래스의 목적: 문자열로 원하는 클래스를 생성

	/*
	* LazySingleton을 사용해야 하는 이유
	static 변수 초기화 시점에 값이 생성됨
	static 변수는 기본적인 변수들은 0, nullptr 등으로 초기화 되지만
	static 타입의 stl 컨테이너는 초기화되지 않아 사용이 불가능하다...
	EntityFactory 안의 static 컨테이너도 마찬가지로 static을 선언할 시 사용이 불가능하다.
	대신 힙에 생성하는 Lazy Singleton 클래스를 통해서 초기화가 가능
	마찬가지로 AtExit도 사용 불가능 -> 직접 메모리 해제를 해주어야 함
	*/
	class EntityFactory
		: public LazySingleton<EntityFactory>
	{
		friend class GameEngine;
		friend class LazySingleton<EntityFactory>;
	private:
		EntityFactory();
		~EntityFactory();

	public:
		std::unique_ptr<Entity> instantiate(const std::string_view key);

		template <typename T>
		std::unique_ptr<T> instantiate(const std::string_view key) {
			std::unique_ptr<Entity> inst = instantiate(key);
			std::unique_ptr<T> ret(dynamic_cast<T*>(inst.get()));
			if (ret) {
				inst.release();
			}
			return ret;
		}

		template <typename T, typename... ARGS>
		bool add_ctor(const std::string_view _name, ARGS&&... _arg) {
			ASSERT_DEBUG(false == _name.empty(), "이름이 비어있습니다.");

			m_ctors[_name] =
				[_arg...]()->std::unique_ptr<Entity> {

				//static_cast가 Entity 에러를 막아줌
				return std::make_unique<T>(_arg...);
				};

			return true;
		}

	private:
		std::unordered_map<std::string_view, std::function<std::unique_ptr<Entity>()>> m_ctors;
	};
}



