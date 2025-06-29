#pragma once
#include "Engine/Game/Entity.h"

#include "Engine/Game/Component/define_Component.h"

#include "Engine/Util/Serialize/Serializable.h"

namespace core
{
	class Scene;
	class Script;
	class Transform;
	class Component;

	class GameObject
		: public Entity
		, public Serializable_Json
	{
		friend class GameObject;

		CLASS_INFO(GameObject, Entity);
		REGISTER_FACTORY(GameObject);
		CLONE_ABLE(GameObject);
		
	public:
		using BaseComponents = std::array<s_ptr<Component>, (size_t)eComponentOrder::END>;
		using Scripts = std::vector<s_ptr<Script>>;

		enum class eState
		{
			InActive,
			Active,
			DestroyReserved,
			Destroy
		};

		GameObject();
		GameObject(const std::string_view _name);
		GameObject(const GameObject& _other);

		virtual ~GameObject();

		virtual void init() override { Super::init(); };
		virtual void Awake();
		virtual void fixed_update();
		virtual void update();
		virtual void final_update();
		virtual void render();
		virtual void frame_end();

		void remove_destroyed();

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;
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
				ret = std::dynamic_pointer_cast<T>(m_baseComponents[(int)T::s_component_order]);
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
			return std::dynamic_pointer_cast<T>(get_script(T::s_static_type_name));
		}

		s_ptr<Script> get_script(const std::string_view _resource_name);

		s_ptr<Transform> get_transform() const;
		s_ptr<Component> get_component(eComponentOrder _type) const { return m_baseComponents[(int)_type]; }

		const BaseComponents& get_components() const { return m_baseComponents; }
		
		const Scripts& get_scripts() const { return m_scripts; }

		void set_name(const std::string_view _Name) { m_name = _Name; }
		const std::string& get_name() const { return m_name; }
		
		void set_active(bool _bActive);
		bool is_active() const { return eState::Active == m_state; }

		eState get_state() const { return m_state; }
		void set_state(eState _state) { m_state = _state; }

		void destroy();
		bool is_destroyed() const { return (eState::DestroyReserved <= m_state); }

		bool is_not_destroyed_on_load() const { return m_bDontDestroyOnLoad; }
		void dont_destroy_on_load(bool _enable) { m_bDontDestroyOnLoad = _enable; }
		
		s_ptr<Scene> get_scene() const { return m_scene.lock(); }
		void set_scene(const s_ptr<Scene>& _scene) { m_scene = _scene; }
		bool is_in_scene() const { return (false == m_scene.expired()); }

		void set_layer(uint32 _layer);
		uint32 get_layer() const { return m_layer; }

		bool is_awaken() const { return m_isAwaken; }

	protected:
		void set_active_internal(bool _bActive);

	private:
		std::string m_name;

		w_ptr<Scene> m_scene;
		uint32 m_layer;

		BaseComponents	m_baseComponents;
		Scripts m_scripts;
		
		eState m_state;

		bool m_isAwaken;
		bool m_bDontDestroyOnLoad;
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
