////#include "Collision3D.h"
//
//
//namespace ehw
//{
//	bool Collision3D::Initialize()
//	{
//		Collision3D::getInstance().Initialize();
//
//		return true;
//	}
//
//	void Collision3D::CollisionUpdate()
//	{
//		Collision3D::getInstance().CollisionUpdate();
//	}
//
//	void Collision3D::EnableRaycast(uint32 leftLayerIndex, uint32 rightLayerIndex, bool enable)
//	{
//		Collision3D::getInstance().EnableRaycast((UINT32)leftLayerIndex, (UINT32)rightLayerIndex, enable);
//	}
//
//	void Collision3D::EnableCollision(uint32 leftLayerIndex, uint32 rightLayerIndex, bool enable)
//	{
//		Collision3D::getInstance().EnableCollision((UINT32)leftLayerIndex, (UINT32)rightLayerIndex, enable);
//	}
//
//	void Collision3D::EnableGravity(bool enable, iScene* pScene, float3 gravity)
//	{
//		gravity = (enable) ? gravity : float3{ 0.f, 0.f, 0.f };
//		Collision3D::getInstance().EnableGravity(enable, pScene, gravity);
//	}
//
//
//	bool Collision3D::Raycast(uint32 srcLayerIndex, const float3& origin, const float3& direction, float maxDistance, RaycastHit* outHit)
//	{
//		float3 normalized{};
//		direction.Normalize(normalized);
//
//		drawRaycast(origin, direction, maxDistance);
//
//		return Collision3D::getInstance().Raycast((UINT32)srcLayerIndex, origin, normalized, maxDistance, outHit);
//	}
//
//
//	void Collision3D::drawRaycast(const float3& origin, const float3& direction, float maxDistance, const float3& color)
//	{
//		//DebugMesh debugInfo{};
//		//debugInfo.type = eColliderType::Line;
//		//debugInfo.position = origin;
//
//		//float3 normDir = direction;
//		//direction.Normalize(normDir);
//
//		////각도를 구해준다
//		//debugInfo.rotation.x = -std::atan2f(normDir.y, normDir.z);
//		//debugInfo.rotation.y = std::asin(normDir.x);
//		//debugInfo.rotation.z = 0.f;
//		//debugInfo.rotation *= gRadianToDegreeFactor;
//
//
//		//float3 rotationAxis = float3::UnitX.Cross(normDir);
//		//float rotationAngle = float3::UnitX.Dot(normDir);
//
//		//// Assuming rotationAxis is normalized
//		//debugInfo.rotation.x = std::atan2(rotationAxis.y, rotationAxis.z);
//		//debugInfo.rotation.y = std::atan2(-rotationAxis.x, std::sqrt(rotationAxis.y * rotationAxis.y + rotationAxis.z * rotationAxis.z));
//		//debugInfo.rotation.z = rotationAngle;
//
//
//
//		//debugInfo.rotation.x = std::acosf(normDir.Dot(float3::UnitX));
//		//debugInfo.rotation.y = std::acosf(normDir.Dot(float3::UnitY));
//		//debugInfo.rotation.z = std::acosf(normDir.Dot(float3::UnitZ));
//
//		//float rotX = std::atan2f(0.f, 0.f) - std::atan2f(direction.y, direction.z);
//		//float rotY = std::atan2f(0.f, 1.f) - std::atan2f(direction.z, direction.x);
//		//float rotZ = std::atan2f(1.f, 0.f) - std::atan2f(direction.x, direction.y);
//		//debugInfo.rotation = float3(rotX, rotY, rotZ);
//
//		//debugInfo.scale = float3(maxDistance);
//		//debugInfo.collisionCount = 1u;
//
//		//renderer::debugMeshes.push_back(debugInfo);
//		//using namespace renderer;
//
//		//float3 forward{};
//		//direction.Normalize(forward);
//		//const Matrix world = Matrix::CreateWorld(origin, forward, float3{ 0.f, 1.f, 0.f });
//		//TransformCB	 buffer{};
//		//buffer.world = Matrix::CreateScale(maxDistance) * world;
//		////const Camera& camera = Application::getInstance().getCamera();
//		//buffer.view = Camera::GetGpuViewMatrix();
//		//buffer.projection = Camera::GetGpuProjectionMatrix();
//
//		//ConstantBuffer* positionBuffer = renderer::constantBuffers[(int)eCBType::Transform];
//		//positionBuffer->SetData(&buffer);
//		//positionBuffer->Bind(eShaderStage::ALL);
//		//
//		//static std::shared_ptr<Mesh>		  line = Resources::Find<Mesh>(L"LineMesh");
//		//static std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"DebugShader");
//
//		//shader->Binds();
//		//line->Render(0u);
//	}
//
//	void Collision3D::createPhysicScene(iScene* scene)
//	{
//		Collision3D::getInstance().createScene(scene);
//	}
//
//	void Collision3D::changePhysicScene(iScene* scene)
//	{
//		Collision3D::getInstance().changeScene(scene);
//	}
//} //namespace ya
