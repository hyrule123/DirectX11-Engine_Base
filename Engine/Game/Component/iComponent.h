#pragma once
#include "Engine/Entity.h"
#include "Engine/Game/Component/define_Component.h"
#include "Engine/Util/Serialize/Serializable.h"

namespace ehw {
	class GameObject;
	class Scene;
	
	//이 클래스를 직접 상속하지 말것!!
	//Component 템플릿 클래스를 상속할것
	class iComponent
		: public Entity
		, public Serializable_Json
	{
		CLASS_NAME(iComponent);
		friend class ComponentManager;
		friend class GameObject;
	public:
		enum class eState
		{
			NotInitialzed,
			NotAwaken,
			Active,
			DestroyReserved,
			Destroy
		};

		iComponent(const std::string_view key, eComponentCategory _type);

		iComponent(const iComponent& _other);
		CLONE_DISABLE(iComponent);

		virtual ~iComponent();

		virtual void init() {}
		virtual void Awake() {}
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void FixedUpdate() {}
		virtual void update() {}
		virtual void final_update() {}

		virtual void frame_end() {}

		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		//레이어 변경시 호출됨. GameObject::Awake() 때 한번 호출됨.
		virtual void OnLayerChange(uint32 _layer) {}

		inline GameObject* gameObject() { return m_ownerGameObject; }
		inline GameObject* gameObject() const { return m_ownerGameObject; }
		inline void Set_gameObject(GameObject* _owner) { m_ownerGameObject = _owner; }

		eComponentCategory GetComponentCategory() const { return m_ComCategory; };

		bool IsStarted() const { return m_isStarted; }

		inline bool IsInitialized() const { return eState::NotInitialzed < m_state; }

		inline bool IsEnabled() const { return m_isEnabled; }
		void SetEnable(bool _bEnable);

		bool IsDestroyed() const { return eState::DestroyReserved <= m_state; }
		inline void Destroy();

		inline eState GetState() const { return m_state; }

		inline bool UpdateDestroyState();

	private:
		inline void SetState(eState _state) { m_state = _state; }

		const eComponentCategory m_ComCategory;
		GameObject* m_ownerGameObject;

		eState m_state;
		bool m_isStarted;
		bool m_isEnabled;
	};

	inline void iComponent::Destroy()
	{
		if (IsDestroyed())
		{
			return;
		}

		m_state = eState::DestroyReserved;
	}

	inline bool iComponent::UpdateDestroyState()
	{
		if (eState::Destroy == m_state)
		{
			return true;
		}
		else if (eState::DestroyReserved == m_state)
		{
			m_state = eState::Destroy;
		}

		return false;
	}
}

