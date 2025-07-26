#pragma once
#include <Engine/Game/Entity.h>

#include <Engine/Game/Component/define_Component.h>

#include <Engine/Util/Serialize/Serializable.h>

namespace core
{
	class Scene;
	class Script;
	class Transform;
	class Component;

	class GameObject
		: public Entity
	{
		friend class GameObject;

		CLASS_INFO(GameObject, Entity);
		REGISTER_FACTORY(GameObject);
		CLONE_ABLE(GameObject);

		NO_SERIALIZE_BINARY;
		NO_SERIALIZE_JSON;
		
	public:
		using BaseComponents = std::array<s_ptr<Component>, (size_t)eComponentOrder::END>;
		using Scripts = std::vector<s_ptr<Script>>;

		enum class eState
		{
			InActive,
			Active,
			DestroyReserved,
			destroy
		};

		GameObject();
		virtual void init() override;

		GameObject(const GameObject& _other);

		virtual ~GameObject();

		
		virtual void awake();
		virtual void fixed_update();
		virtual void update();
		virtual void final_update();
		virtual void render();
		virtual void frame_end();

		void remove_destroyed();
	public:
		s_ptr<Component> add_component(const s_ptr<Component>& _pCom);

		template <typename T>
		s_ptr<T> add_component();
		s_ptr<Component> add_component(const std::string_view _resource_name);

		template <typename T>
		s_ptr<T> get_component() {
			static_assert(std::is_base_of_v<Component, T>, "Component의 하위 클래스가 아닙니다.");

			s_ptr<T> ret = nullptr;
			//Script는 이름을 통한 비교
			if constexpr (std::is_base_of_v<Script, T>)
			{
				ret = get_script<T>();
			}
			//이외의 
			else //constexpr
			{
				ret = std::dynamic_pointer_cast<T>(m_base_components[(int)T::s_component_order]);
			}

			return ret;
		}

		template <typename T>
		s_ptr<T> add_script() {
			static_assert(std::is_base_of_v<Script, T>, "Script의 하위 클래스가 아닙니다.");
			return add_component<T>();
		};

		s_ptr<Script> add_script(const std::string_view _resource_name);

		template <typename T>
		s_ptr<T> get_script() {
			static_assert(std::is_base_of_v<Script, T>, "Script의 하위 클래스가 아닙니다.");
			return std::dynamic_pointer_cast<T>(get_script(T::s_concrete_class_name));
		}

		s_ptr<Script> get_script(const std::string_view _resource_name);

		s_ptr<Transform> get_transform() const;
		s_ptr<Component> get_component(eComponentOrder _type) const { return m_base_components[(int)_type]; }

		const BaseComponents& get_components() const { return m_base_components; }
		
		const Scripts& get_scripts() const { return m_scripts; }

		void set_name(const std::string_view _Name) { m_name = _Name; }
		const std::string& get_name() const { return m_name; }
		
		void set_active(bool _bActive);
		bool is_active() const { return eState::Active == m_state; }

		eState get_state() const { return m_state; }
		void set_state(eState _state) { m_state = _state; }

		void destroy();
		bool is_destroyed() const { return (eState::DestroyReserved <= m_state); }

		bool is_not_destroyed_on_load() const { return m_b_dont_destroy_on_load; }
		void dont_destroy_on_load(bool _enable) { m_b_dont_destroy_on_load = _enable; }
		
		s_ptr<Scene> get_scene() const { return m_scene.lock(); }
		void set_scene(const s_ptr<Scene>& _scene) { m_scene = _scene; }
		bool is_in_scene() const { return (false == m_scene.expired()); }

		void set_layer(uint32 _layer);
		uint32 get_layer() const { return m_layer; }

		bool is_awaken() const { return m_b_awake; }

	protected:
		void set_active_internal(bool _bActive);

	private:
		std::string m_name;

		w_ptr<Scene> m_scene;
		uint32 m_layer;

		BaseComponents	m_base_components;
		Scripts m_scripts;
		
		eState m_state;

		bool m_b_awake;
		bool m_b_dont_destroy_on_load;
	};


	template <typename T>
	s_ptr<T> GameObject::add_component()
	{
		static_assert(std::is_base_of_v<Component, T>, "Component의 하위 클래스가 아닙니다.");

		s_ptr<T> ret = std::make_shared<T>();

		ret = std::static_pointer_cast<T>(add_component(ret));

		return ret;
	}
}
