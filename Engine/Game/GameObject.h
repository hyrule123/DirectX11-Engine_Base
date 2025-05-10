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
		using BaseComponents = std::array<Component*, (size_t)eComponentCategory::BaseComponentEnd>;
		using Scripts = std::vector<Script*>;

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

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		void Awake();
		void fixed_update();
		void update();
		void final_update();
		void render();
		void frame_end();
		void RemoveDestroyed();

	public:
		Component* AddComponent(Component* _pCom);
		template <typename T> requires std::is_base_of_v<Component, T>
		T* AddComponent();
		Component* AddComponent(const std::string_view _resource_name);

		template <typename T>
		T* GetComponent();

		template <typename T> requires std::is_base_of_v<Script, T>
		T* AddScript();
		Script* AddScript(const std::string_view _resource_name);

		template <typename T>
		T* GetScript();

		Script* GetScript(const std::string_view _resource_name);

		Component* GetComponent(eComponentCategory _type) { return m_baseComponents[(int)_type]; }
		Transform* transform();

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
		
		Scene* scene() { return m_scene; }
		Scene* scene() const { return m_scene; }
		void SetScene(Scene* _scene) { m_scene = _scene; }
		bool IsInScene() const { return (nullptr != m_scene); }

		void SetLayer(uint32 _layer);
		uint32 GetLayer() const { return m_layer; }

		bool IsAwaken() const { return m_isAwaken; }

	protected:
		void SetActiveInternal(bool _bActive);

	private:
		std::string m_name;

		Scene* m_scene;
		uint32 m_layer;

		BaseComponents	m_baseComponents;
		Scripts m_scripts;
		
		eState m_state;

		bool m_isAwaken;
		bool m_bDontDestroyOnLoad;
	};


	template <typename T> requires std::is_base_of_v<Component, T>
	T* GameObject::AddComponent()
	{
		T* ret = new T;

		eComponentCategory order = ret->GetComponentCategory();

		if (false == IsComponentCategoryValid(order))
		{
			delete ret;
			ret = nullptr;
			return ret;
		}

		//Component로 캐스팅해서 AddComponent 함수 호출 후 다시 T타입으로 바꿔서 반환
		return static_cast<T*>(AddComponent(ret));
	}

	template <typename T>
	T* GameObject::GetComponent()
	{
		T* pCom = nullptr;

		//Script는 이름을 통한 비교
		if constexpr (std::is_base_of_v<Script, T>)
		{
			pCom = GetScript<T>();
		}

		//Script 아니고 Base Component 타입으로 반환을 요청한 경우
		else if constexpr (T::template IsBaseComponentType<T>())
		{
			//Base Component 타입으로 요청했을 경우에는 static cast 후 반환
			pCom = static_cast<T*>(m_baseComponents[(int)T::s_order]);
		}

		//Base Component 타입으로 반환이 아닐 경우 타입 검증 후 반환
		else //constexpr
		{
			pCom = dynamic_cast<T*>(m_baseComponents[(int)T::s_order]);
		}

		return pCom;
	}

	template<typename T> requires std::is_base_of_v<Script, T>
	inline T* GameObject::AddScript()
	{
		return AddComponent<T>();
	};


	template<typename T>
	inline T* GameObject::GetScript()
	{
		return static_cast<T*>(GetScript(T::concrete_class_name));
	};

	inline Transform* GameObject::transform()
	{
		return (Transform*)m_baseComponents[(int)eComponentCategory::Transform];
	};
}
