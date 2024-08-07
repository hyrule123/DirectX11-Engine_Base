#pragma once
#include "Engine/Entity.h"
#include "Engine/Scene/Component/define_Component.h"
#include "Engine/Util/Serialize/Serializable.h"

namespace ehw {
	class GameObject;
	class Scene;

	class iComponent
		: public Entity
		, public Serializable_Json
	{
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

		iComponent(eComponentCategory _type);

		iComponent(const iComponent& _other);
		CLONE_DISABLE(iComponent);

		virtual ~iComponent();

		virtual void Init() {}
		virtual void Awake() {}
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void FixedUpdate() {}
		virtual void Update() {}
		virtual void FinalUpdate() {}

		virtual void FrameEnd() {}

		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		//레이어 변경시 호출됨. GameObject::Awake() 때 한번 호출됨.
		virtual void OnLayerChange(uint32 _layer) {}

		inline GameObject* gameObject() { return m_ownerGameObject; }
		inline GameObject* gameObject() const { return m_ownerGameObject; }
		inline void Set_gameObject(GameObject* _owner) { m_ownerGameObject = _owner; }

		eComponentCategory GetComponentCategory() const { return m_ComCategory; };

		inline void SetComponentTypeID(size_t _comTypeID) { m_ComTypeID = _comTypeID; }
		size_t GetComponentTypeID() const { return m_ComTypeID; };

		bool IsStarted() const { return m_isStarted; }

		inline bool IsInitialized() const { return eState::NotInitialzed < m_state; }

		inline bool IsEnabled() const { return m_isEnabled; }
		void SetEnable(bool _bEnable);

		bool IsDestroyed() const { return eState::DestroyReserved <= m_state; }
		inline void Destroy();

		inline eState GetState() const { return m_state; }

		inline bool UpdateDestroyState();

		template <typename T>
		static inline size_t GetComponentTypeID();
	private:
		static inline size_t GetNextComponentTypeID();
		inline void SetState(eState _state) { m_state = _state; }

		const eComponentCategory m_ComCategory;
		size_t m_ComTypeID;
		GameObject* m_ownerGameObject;

		eState m_state;
		bool m_isStarted;
		bool m_isEnabled;
	};


	template<typename T>
	inline size_t iComponent::GetComponentTypeID()
	{
		static_assert(std::is_base_of_v<iComponent, T>);
		static size_t typeId = GetNextComponentTypeID();
		return typeId;
	}

#pragma push_macro("max")
#undef max
	inline size_t iComponent::GetNextComponentTypeID()
	{
		static size_t lastID = std::numeric_limits<size_t>::max();
		return ++lastID;
	}
#pragma pop_macro("max")


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

