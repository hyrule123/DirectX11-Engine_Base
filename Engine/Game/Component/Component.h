#pragma once
#include <Engine/Game/Entity.h>
#include <Engine/Game/Component/define_Component.h>
#include <Engine/Util/Serialize/Serializable.h>

/*
************************ 주의 ********************
Component 클래스를 직접 상속 X, BaseComponent를 직접 상속해서 반드시 enum과 연동시킬 것
Transform 에서 예시 확인
*/


//static_assert(is_base_component<type>, "Component의 하위 클래스가 아닙니다.");

#define BASE_COMPONENT(component_order) \
public: constexpr inline static const eComponentOrder s_component_order = component_order

namespace core {
	class GameObject;
	class Scene;
	
	class Component
		: public Entity
		, public Serializable_Json
	{
		CLASS_INFO(Component, Entity);
		CLONE_DISABLE(Component);

		friend class ComponentManager;
		friend class GameObject;
		
	public:
		enum class eState
		{
			NotInitialzed,
			NotAwaken,
			Active,
			DestroyReserved,
			destroy
		};

		Component(const std::string_view key, eComponentOrder _type);
		Component(const Component& _other);
		virtual ~Component();

		virtual void init() override;
		virtual void awake() {}
		virtual void on_enable() {}
		virtual void start() {}
		virtual void fixed_update() {}
		virtual void update() {}
		virtual void final_update() {}

		virtual void frame_end() {}

		virtual void on_disable() {}
		virtual void on_destroy() {}

		//레이어 변경시 호출됨. GameObject::Awake() 때 한번 호출됨.
		virtual void on_layer_change(uint32 _layer) {}

		inline s_ptr<GameObject> get_owner() const { return m_ownerGameObject.lock(); }
		inline void set_game_object(const s_ptr<GameObject>& _owner) { m_ownerGameObject = _owner; }

		eComponentOrder get_component_order() const { return m_component_order; };

		bool is_started() const { return m_b_start; }

		inline bool is_initialized() const { return eState::NotInitialzed < m_state; }

		inline bool is_enabled() const { return m_b_enable; }
		void set_enable(bool _bEnable);

		bool is_destroyed() const { return eState::DestroyReserved <= m_state; }
		inline void destroy() {
			if (is_destroyed()) { return; }
			m_state = eState::DestroyReserved;
		}

		inline eState get_state() const { return m_state; }

		bool update_destroy_state();

	private:
		inline void set_state(eState _state) { m_state = _state; }

		const eComponentOrder m_component_order;
		w_ptr<GameObject> m_ownerGameObject;

		eState m_state;
		bool m_b_start;
		bool m_b_enable;
	};

	template <typename T>
	concept is_base_component = requires(T t)
	{
		std::is_base_of_v<Component, T>;
		{ t.get_component_order() } -> std::same_as<eComponentOrder>;
	};
}

