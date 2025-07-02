#pragma once
#include <Engine/Common.h>
#include <type_traits>


// **************** 사용 가능한 PROPERTY *****************************


//CLONE_ABLE 선언시 반드시 복사생성자를 재정의해줘야 함
#define CLONE_ABLE(_type) \
public: virtual s_ptr<Entity> Clone() override { \
return std::make_shared<_type>(*this); \
}

//Clone을 지원하지 않을 경우 nullptr이 반환된다.
#define CLONE_DISABLE(_type) \
private: virtual s_ptr<Entity> Clone() override { return nullptr; }

//고유 이름으로 생성자 등록, 웬만하면 쓸 일 없을듯?
#define REGISTER_FACTORY_UNIQUE(_class, _key) \
private: static inline const bool UNIQUE_VAR(unused) = EntityFactory::get_inst().add_ctor<_class>(_key)

#define REGISTER_FACTORY(_class) REGISTER_FACTORY_UNIQUE(_class, _class::s_static_type_name)

namespace core
{
	class Entity
		: public std::enable_shared_from_this<Entity>
	{
	public:
		Entity(const std::string_view _static_type_name);
		virtual void init();
		virtual ~Entity();

		Entity(const Entity& _other);
		//Clone을 지원하지 않을 경우 nullptr이 반환된다.
		virtual s_ptr<Entity> Clone() { return nullptr; }

		//런타임 멤버함수
		const std::string_view get_static_type_name() const {
			return m_static_type_name;
		}
		uint32 GetID() const { return m_ID; }

	private:
		const uint32 m_ID;
		const std::string_view m_static_type_name;

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
	{
		friend class GameEngine;

		DECLARE_SINGLETON(EntityFactory);
	private:
		EntityFactory();
		~EntityFactory();

	public:
		s_ptr<Entity> instantiate(const std::string_view key);

		template <typename T, typename... ARGS>
		bool add_ctor(const std::string_view _name, ARGS&&... _arg) {
			ASSERT_DEBUG(false == _name.empty(), "이름이 비어있습니다.");

			m_ctors[_name] =
				[_arg...]()->s_ptr<Entity> {
				return std::make_shared<T>(_arg...);
				};

			return true;
		}

	private:
		std::unordered_map<std::string_view, std::function<s_ptr<Entity>()>> m_ctors;
	};

	//전역 변수
	inline s_ptr<Entity> new_entity(const std::string_view _class_name)
	{
		return EntityFactory::get_inst().instantiate(_class_name);
	}
	template <typename T> 
	inline s_ptr<T> new_entity()
	{
		static_assert(std::is_base_of_v<Entity, T>, "Entity를 상속받은 클래스가 아님");
		s_ptr<T> new_ent = std::make_shared<T>();
		new_ent->init();
		//혹시나 모를 fail safe - T 클래스에 이름을 등록해놓지 않았을 경우 대참사가 날수도 있음
		return new_ent;
	}
}



