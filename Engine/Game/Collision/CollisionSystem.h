#pragma once
#include <Engine/define_enum.h>
#include <Engine/Common.h>

#include <Engine/Util/type_traits_Ex.h>

#include <Engine/DefaultShader/Debug/Debug.hlsli>

//업데이트 순서
//1. SceneManager::update: 각 Collider들이 자신의 정보를 등록한다.
//2. CollisionSystem::final_update: 등록된 Collider에 대한 충돌검사를 시행한다.(여기서 충돌 관련 연산이 완료 되어야함)
//3. SceneManager::final_update: 최종적으로 Trasnfrom 위치를 갱신하고, 각 컴포넌트의 렌더링 준비를 마친다.

//아이디어 1
//Collider의 ID(UINT32) 두개를 붙여서(union) uint 형태를 만들고, 이 값을 이용해 Collision Pair unordered_map을 생성

//아이디어 2(현재 채택)
//개별 Collider 컴포넌트가 현재 충돌중인 상대방 Collider 주소를 관리

namespace core
{
	class Scene;
	class Collision2D;
	class Collision3D;
	class StructBuffer;
	class Mesh;
	class Material;
	class CollisionSystem
	{
		friend class Scene;
	public:
		CollisionSystem(Scene* const _ownerScene);
		~CollisionSystem();

		inline Collision2D* GetCollision2D() {
			if (nullptr == m_collision2D_manager) {
				CreateCollision2D();
			}
			return m_collision2D_manager.get(); 
		}
		inline Collision3D* get_collision_manager_3D() { 
			if (nullptr == m_collision3D_manager) {
				CreateCollision3D();
			}
			return m_collision3D_manager.get(); 
		}

		template <type_traits_Ex::is_enum_class_v T>
		inline void SetCollisionMask(T _left, T _right, bool _enable = true);
		void SetCollisionMask(uint _layerA, uint _layerB, bool _isEnable);
		
		inline const std::array<std::bitset<g_maxLayer>, g_maxLayer>&
			GetCollisionMask() { return m_collision_mask; }
		inline std::bitset<g_maxLayer> GetLayerCollisionMask(uint _layer) const;


		template <type_traits_Ex::is_enum_class_v T>
		inline void SetRaycastMask(T _left, T _right, bool _enable = true);
		void SetRaycastMask(uint _layerA, uint _layerB, bool _isEnable);
		inline const std::array<std::bitset<g_maxLayer>, g_maxLayer>&
			GetRaycastMask() { return m_raycast_mask; };
		inline std::bitset<g_maxLayer> GetLayerRaycastMask(uint _layer) const;

		inline Scene* GetOwnerScene() { return m_owner; }

		//Editor 프로젝트에서 호출됨.
		void render();

	private:
		void CreateCollision2D();
		void CreateCollision3D();

		void fixed_update();
		void frame_end();

	private:
		Scene* const m_owner;

		//레이어끼리의 상호작용 여부를 담아놓은 이차원 배열
		std::array<std::bitset<g_maxLayer>, g_maxLayer> m_collision_mask;
		std::array<std::bitset<g_maxLayer>, g_maxLayer> m_raycast_mask;

		std::unique_ptr<Collision2D> m_collision2D_manager;
		std::unique_ptr<Collision3D> m_collision3D_manager;
	};

	template<type_traits_Ex::is_enum_class_v T>
	inline void CollisionSystem::SetCollisionMask(T _left, T _right, bool _enable)
	{
		SetCollisionMask(static_cast<uint>(_left), static_cast<uint>(_right), _enable);
	}

	inline std::bitset<g_maxLayer> CollisionSystem::GetLayerCollisionMask(uint _layer) const
	{
		std::bitset<g_maxLayer> ret{};

		if (_layer < g_maxLayer)
		{
			ret = m_collision_mask[_layer];
		}

		return ret;
	}
	inline std::bitset<g_maxLayer> CollisionSystem::GetLayerRaycastMask(uint _layer) const
	{
		std::bitset<g_maxLayer> ret{};

		if (_layer < g_maxLayer)
		{
			ret = m_raycast_mask[_layer];
		}

		return ret;
	}

	template<type_traits_Ex::is_enum_class_v T>
	inline void CollisionSystem::SetRaycastMask(T _left, T _right, bool _enable)
	{
		SetRaycastMask(static_cast<uint>(_left), static_cast<uint>(_right), _enable);
	}
}
