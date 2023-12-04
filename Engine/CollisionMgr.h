#pragma once
#include "EnumFlags.h"
#include "ICollider2D.h"

namespace ehw
{
	union union_ColliderID
	{
		struct
		{
			UINT32 Left;
			UINT32 Right;
		};
		UINT64 Id;
	};

	//	class Scene;
	class CollisionMgr
	{
		friend class Application;
	public:
		static void Update();
		static void InternalUpdate();
		static void Render();

		static void CollisionLayerCheck(eLayerType _left, eLayerType _right, bool _enable = true);
		static void LayerCollision(class IScene* _scene, eLayerType _left, eLayerType _right);
		static void ColliderCollision(ICollider2D* _left, ICollider2D* _right);
		static bool Intersect(ICollider2D* _left, ICollider2D* _right);

	private:
		static void Init();
		static void Release();

	private:
		static std::bitset<(uint)eLayerType::END> mLayerCollisionMatrix[(uint)eLayerType::END];
		static std::map<UINT64, bool> mCollisionMap;
	};
}
