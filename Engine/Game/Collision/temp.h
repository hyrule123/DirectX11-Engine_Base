#pragma once
#include "Engine/CommonType.h"

namespace ehw
{
	//전방 선언
	struct RaycastHit;
	class iScene;

	class temp final
	{
	public:
		bool Initialize();
		void CollisionUpdate();

		void EnableRaycast(uint32 leftLayerIndex, uint32 rightLayerIndex, bool enable);
		void EnableCollision(uint32 leftLayerIndex, uint32 rightLayerIndex, bool enable);
		void EnableGravity(bool enable, iScene* pScene, float3 gravity = float3(0.f, -9.8f, 0.f));
		bool Raycast(uint32 srcLayerIndex, const float3& origin, const float3& direction, float maxDistance, RaycastHit* outHit);

		void drawRaycast(const float3& origin, const float3& direction, float maxDistance, const float3& color = float3{ 1.f, 0.f, 0.f });

		void createPhysicScene(iScene* scene);
		void changePhysicScene(iScene* scene);

	private:
	};




} //namespace ehw
