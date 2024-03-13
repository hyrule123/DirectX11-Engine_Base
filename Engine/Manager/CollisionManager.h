#pragma once
#include "Engine/define_enum.h"
#include "Engine/Util/type_traits_Ex.h"
#include "Engine/CommonType.h"

#include "Engine/Manager/Collision/Collision2DManager.h"
#include "Engine/Manager/Collision/Collision3DManager.h"

#include <unordered_map>
#include <array>
#include <bitset>

namespace ehw
{
	union union_ColliderID
	{
		struct
		{
			uint32 Left;
			uint32 Right;
		};
		uint64 Id;
	};

	class iCollider2D;

	//	class Scene;
	class CollisionManager
	{
		friend class GameEngine;
	public:
		static void LateUpdate();
		static void Render();

		//static void RegisterCollider(const std::shared_ptr<iCollider2D>& _col2D);

		static void LayerCollision(class iScene* _scene, uint32 _left, uint32 _right);
		static void ColliderCollision(const std::shared_ptr<iCollider2D>& _left, const std::shared_ptr<iCollider2D>& _right);
		static bool Intersect(const std::shared_ptr<iCollider2D>& _left, const std::shared_ptr<iCollider2D>& _right);
		
		template <type_traits_Ex::is_enum_class_v T>
		inline void SetCollisionInteraction(T _left, T _right, bool _enable = true);
		const std::array<std::bitset<g_maxLayer>, g_maxLayer>& GetCollisionMasks() const { return m_collisionMask; }

	private:
		static void Init();
		static void Release();

	private:
		static std::array<std::bitset<g_maxLayer>, g_maxLayer> m_collisionMask;

		static Collision2DManager m_col2DManager;
		static Collision3DManager m_col3DManager;
	};

	template<type_traits_Ex::is_enum_class_v T>
	inline void CollisionManager::SetCollisionInteraction(T _left, T _right, bool _enable)
	{
		uint left = static_cast<uint>(_left);
		uint right = static_cast<uint>(_right);

		m_collisionMask[left][right] = _enable;
		m_collisionMask[right][left] = _enable;
	}
}
