#pragma once

#include "Engine/Util/EnumFlags.h"
#include "Engine/Game/Component/Collider/iCollider2D.h"

#include <map>

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
	class CollisionManager
	{
		friend class Application;
	public:
		static void Update();
		static void InternalUpdate();
		static void Render();

		static void CollisionLayerCheck(eLayer _left, eLayer _right, bool _enable = true);
		static void LayerCollision(class iScene* _scene, eLayer _left, eLayer _right);
		static void ColliderCollision(const std::shared_ptr<iCollider2D>& _left, const std::shared_ptr<iCollider2D>& _right);
		static bool Intersect(const std::shared_ptr<iCollider2D>& _left, const std::shared_ptr<iCollider2D>& _right);

	private:
		static void Init();
		static void Release();

	private:
		static std::bitset<(uint)eLayer::END> mLayerCollisionMatrix[(uint)eLayer::END];
		static std::map<UINT64, bool> mCollisionMap;
	};
}
