#pragma once
#include <Engine/Common.h>
#include <type_traits>

namespace core
{

// **************** 사용 가능한 PROPERTY *****************************
//참고: 여러 클래스 계층에서 동일한 변수를 가지고 있을때 명시적으로 호출하지 않은 경우 가장 낮은 클래스의 변수를 참조한다.
//Component(base), Transform(child) 
// -> Component::s_concrete_class_name = "Component"
// -> s:concrete_class_name = "Transform"
#define CLASS_INFO(_class_, _super_) \
public:\
virtual std::string_view get_concrete_class_name_internal() const override {\
	return s_concrete_class_name;\
}\
virtual uint32 get_concrete_class_ID_internal() const override {\
return s_concrete_class_ID;\
}\
static inline constexpr const std::string_view s_concrete_class_name = #_class_;\
static inline const uint32 s_concrete_class_ID = get_next_class_ID();\
private:\
using Super = _super_



//CLONE_ABLE 선언시 반드시 복사생성자를 재정의해줘야 함
#define CLONE_ABLE(_type) \
public: virtual s_ptr<Entity> clone() override { \
return new_entity<_type>(*this); \
}

//Clone을 지원하지 않을 경우 nullptr이 반환된다.
#define CLONE_DISABLE(_type) \
private: virtual s_ptr<Entity> clone() override { return nullptr; }

//고유 이름으로 생성자 등록, 웬만하면 쓸 일 없을듯?
#define REGISTER_FACTORY_UNIQUE(_class, _key) \
private: static inline const bool m_class_ID = EntityFactory::get_inst().add_ctor<_class>(_key)

#define REGISTER_FACTORY(_class) REGISTER_FACTORY_UNIQUE(_class, _class::s_concrete_class_name)


	class Entity
		: public std::enable_shared_from_this<Entity>
	{
	public:
		Entity();
		virtual void init();
		virtual ~Entity();

		Entity(const Entity& _other);

		//Clone을 지원하지 않을 경우 nullptr이 반환된다.
		virtual s_ptr<Entity> clone() { return nullptr; }
		
		uint32 get_instance_ID() const { return m_instance_ID; }
		uint32 get_concrete_class_ID() const { return m_concrete_class_ID; }
		std::string_view get_concrete_class_name() const { return m_concrete_class_name; }

	protected:
		static uint32 get_next_class_ID() { return ++s_next_class_ID; }

		//오버헤드를 줄이기 위해서 virtual은 init에서 한번만 호출하고, 이후 호출은 inline 함수로 되도록 하였음.
		virtual std::string_view get_concrete_class_name_internal() const {
			return "Entity";
		}
		virtual uint32 get_concrete_class_ID_internal() const {
			return 0;
		}

	private:
		const uint32 m_instance_ID;
		
		uint32 m_concrete_class_ID;
		std::string_view m_concrete_class_name;

		static uint32 s_next_instance_ID;
		static uint32 s_next_class_ID;
	};

	//전방 선언
	inline s_ptr<Entity> new_entity(const std::string_view _class_name);
	template <typename T, typename... ARGS>
	inline s_ptr<T> new_entity(ARGS&& ...args)
	{
		static_assert(std::is_base_of_v<Entity, T>, "Entity derived class ONLY");
		s_ptr<T> new_ent = std::make_shared<T>(args...);
		new_ent->init();
		return new_ent;
	}


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
		void init();

		~EntityFactory();

	public:
		s_ptr<Entity> instantiate(const std::string_view key);

		template <typename T, typename... ARGS>
		uint32 add_ctor(const std::string_view _name, ARGS&&... _arg) {
			ASSERT_DEBUG(false == _name.empty(), "이름이 비어있습니다.");

			m_ctors[_name] =
				[_arg...]()->s_ptr<Entity> {
				return new_entity<T>(_arg...);
				};

			return ++m_next_class_ID;
		}

	private:
		std::unordered_map<std::string_view, std::function<s_ptr<Entity>()>> m_ctors;
		uint32 m_next_class_ID;
	};

	//전역 변수
	inline s_ptr<Entity> new_entity(const std::string_view _class_name)
	{
		return EntityFactory::get_inst().instantiate(_class_name);
	}
}



