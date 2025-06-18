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

		virtual void init();
		virtual void Awake();
		virtual void fixed_update();
		virtual void update();
		virtual void final_update();
		virtual void render();
		virtual void frame_end();

		void RemoveDestroyed();

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;
	public:
		s_ptr<Component> AddComponent(const s_ptr<Component>& _pCom);

		template <typename T>
		s_ptr<T> AddComponent();
		s_ptr<Component> AddComponent(const std::string_view _resource_name);

		template <typename T>
		s_ptr<T> GetComponent() {
			static_assert(std::is_base_of_v<Component, T>, "Component의 하위 클래스가 아닙니다.");

			s_ptr<T> ret = nullptr;
			//Script는 이름을 통한 비교
			if constexpr (std::is_base_of_v<Script, T>)
			{
				ret = GetScript<T>();
			}
			//이외의 
			else //constexpr
			{
				ret = std::dynamic_pointer_cast<T>(m_baseComponents[(int)T::s_component_order]);
			}

			return ret;
		}

		template <typename T>
		s_ptr<T> AddScript() {
			static_assert(std::is_base_of_v<Script, T>, "Script의 하위 클래스가 아닙니다.");
			return AddComponent<T>();
		};

		s_ptr<Script> AddScript(const std::string_view _resource_name);

		template <typename T>
		s_ptr<T> GetScript() {
			static_assert(std::is_base_of_v<Script, T>, "Script의 하위 클래스가 아닙니다.");
			return std::dynamic_pointer_cast<T>(GetScript(T::s_concrete_class_name));
		}

		s_ptr<Script> GetScript(const std::string_view _resource_name);

		s_ptr<Component> GetComponent(eComponentOrder _type) { return m_baseComponents[(int)_type]; }

		const BaseComponents& GetComponents() const { return m_baseComponents; }
		
		const Scripts& GetScripts() const { return m_scripts; }

		void SetName(const std::string_view _Name) { m_name = _Name; }
		const std::string& GetName() const { return m_name; }
		
		void SetActive(bool _bActive);
		bool IsActive() const { return eState::Active == m_state; }

		eState GetState() const { return m_state; }
		void SetState(eState _state) { m_state = _state; }

		void Destroy();
		bool IsDestroyed() const { return (eState::DestroyReserved <= m_state); }

		bool IsDontDestroyOnLoad() const { return m_bDontDestroyOnLoad; }
		void DontDestroyOnLoad(bool _enable) { m_bDontDestroyOnLoad = _enable; }
		
		s_ptr<Scene> get_scene() const { return m_scene.lock(); }
		void set_scene(const s_ptr<Scene>& _scene) { m_scene = _scene; }
		bool IsInScene() const { return (false == m_scene.expired()); }

		void SetLayer(uint32 _layer);
		uint32 GetLayer() const { return m_layer; }

		bool IsAwaken() const { return m_isAwaken; }

	protected:
		void SetActiveInternal(bool _bActive);

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
	s_ptr<T> GameObject::AddComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "Component의 하위 클래스가 아닙니다.");

		s_ptr<T> ret = std::make_shared<T>();

		ret = std::static_pointer_cast<T>(AddComponent(ret));

		return ret;
	}
}
