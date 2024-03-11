#include "Engine/Game/Component/Transform/Com_Transform.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Manager/RenderManager.h"

#include "Engine/GPU/ConstBuffer.h"

#include "Engine/GameEngine.h"






namespace ehw
{
	Com_Transform::Com_Transform()
		: m_localScale(float3::One)
		, m_localSize(float3::One)
		, m_localRotation(Quaternion::Identity)
		, m_localPosition(float3::Zero)
		, m_localDirection{ float3::Right, float3::Up, float3::Forward }
		, m_localMatrix(MATRIX::Identity)
		, m_worldScale(float3::One)
		, m_worldRotation(math::Quaternion::Identity)
		, m_worldDirection{ float3::Right, float3::Up, float3::Forward }
		, m_worldMatrix(MATRIX::Identity)
		, m_bNeedUpdateLocalMatrix(true)
		, m_bNeedUpdateWorldValue(true)
		, m_bNeedUpdateWorldMatrix(true)
		, m_bTransformUpdated(true)
		, m_parent()
		, m_childs()
	{
	}

	Com_Transform::~Com_Transform()
	{
	}

	void Com_Transform::InternalUpdate()
	{
		//World Value 업데이트
		UpdateWorldValue();

		//아래 함수에 UpdateLocalMatrix()가 포함되어 있음.
		UpdateWorldMatrix();
	}


	void Com_Transform::BindData()
	{
		tCB_Transform cbData{};
		cbData.World = m_worldMatrix;
		cbData.View = Com_Camera::GetGpuViewMatrix();
		cbData.InverseView = Com_Camera::GetGpuViewInvMatrix();
		cbData.Projection = Com_Camera::GetGpuProjectionMatrix();
		cbData.WorldView = cbData.World * cbData.View;
		cbData.WVP = cbData.WorldView * cbData.Projection;

		ConstBuffer* cb = RenderManager::GetConstBuffer(eCBType::Transform);
		cb->SetData(&cbData);
		cb->BindData(eShaderStageFlag::ALL);
	}



	bool Com_Transform::UpdateLocalMatrix()
	{
		if (false == m_bNeedUpdateLocalMatrix)
		{
			return false;
		}
		m_bNeedUpdateLocalMatrix = false;
		m_bTransformUpdated = true;

		//1. 크기행렬
		m_localMatrix = MATRIX::CreateScale(m_localScale);

		//2. 회전행렬
		MATRIX matRot = MATRIX::CreateFromQuaternion(m_localRotation);
		m_localMatrix *= matRot;

		//2-1. 회전행렬으로부터 직관적 방향을 계산한다.
		m_localDirection[(int)eDirection::Forward] = matRot.Forward();
		m_localDirection[(int)eDirection::Right] = matRot.Right();
		m_localDirection[(int)eDirection::Up] = matRot.Up();

		//3. 이동행렬
		m_localMatrix *= MATRIX::CreateTranslation(m_localPosition);

		return true;
	}

	void Com_Transform::GetGameObjectHierarchy_Recursive(std::vector<std::shared_ptr<GameObject>>& _retObjects)
	{
		_retObjects.push_back(GetOwner()->shared_from_this_T<GameObject>());
		for (size_t i = 0; i < m_childs.size(); ++i)
		{
			m_childs[i]->GetGameObjectHierarchy_Recursive(_retObjects);
		}
	}

	bool Com_Transform::UpdateWorldValue()
	{
		std::shared_ptr<Com_Transform> parent = m_parent.lock();
		
		
		if (parent)
		{
			//parent도 WorldValue 업데이트를 진행.(or)
			m_bNeedUpdateWorldValue |= parent->UpdateWorldValue();
		}

		//parent가 갱신되었을 경우 or 자신의 WorldValue 갱신이 필요할 경우 갱신
		if (false == m_bNeedUpdateWorldValue)
		{
			return false;
		}
		m_bNeedUpdateWorldValue = false;
		m_bTransformUpdated = true;

		m_worldScale = m_localScale;
		m_worldRotation = m_localRotation;

		//Value
		if (parent)
		{
			if (false == m_bIgnoreParentScale)
			{
				m_worldScale *= parent->GetWorldScale();
			}
			if (false == m_bIgnoreParentRotation)
			{
				m_worldRotation *= parent->GetWorldRotation();
			}
		}

		return true;
	}

	bool Com_Transform::UpdateWorldMatrix()
	{
		std::shared_ptr<Com_Transform> parent = m_parent.lock();

		m_bNeedUpdateWorldMatrix |= UpdateLocalMatrix();
		if (parent)
		{
			m_bNeedUpdateWorldMatrix |= parent->UpdateWorldMatrix();
		}

		if (false == m_bNeedUpdateWorldMatrix)
		{
			return false;
		}
		m_bNeedUpdateWorldMatrix = false;
		m_bTransformUpdated = true;

		m_worldMatrix = m_localMatrix;
		if (parent)
		{
			MATRIX parentMat = MATRIX::Identity;
			constexpr size_t rowErasebytes = sizeof(float) * 3;

			std::shared_ptr<Com_Transform> parent = m_parent.lock();
			//크기 무시
			if (m_bIgnoreParentScale)
			{
				//크기 무시 + 회전 무시: 0, 0부터 3, 3까지 밀어버리고 단위행렬로 변경
				if (m_bIgnoreParentRotation)
				{
					parentMat = parent->GetWorldMatrix();

					memset(parentMat.m[0], 0, rowErasebytes);
					memset(parentMat.m[1], 0, rowErasebytes);
					memset(parentMat.m[2], 0, rowErasebytes);
					parentMat._11 = 1.f;
					parentMat._22 = 1.f;
					parentMat._33 = 1.f;
				}

				//크기 무시 + 회전 반영
				else
				{
					//정규화하면 크기정보가 제거된다.
					parentMat *= parent->GetWorldMatrix();
					parentMat.Right(parentMat.Right().Normalize());
					parentMat.Up(parentMat.Up().Normalize());
					parentMat.Forward(parentMat.Forward().Normalize());
				}
			}

			//크기 반영
			else
			{
				//크기 반영 + 회전 무시
				if (m_bIgnoreParentRotation)
				{
					parentMat = parent->GetWorldMatrix();

					//우선 크기 + 회전 정보가 있는 부분을 초기화 한다.
					memset(parentMat.m[0], 0, rowErasebytes);
					memset(parentMat.m[1], 0, rowErasebytes);
					memset(parentMat.m[2], 0, rowErasebytes);

					//WorldScale을 집어넣어준다.
					const float3& worldScale = parent->GetWorldScale();
					parentMat._11 = worldScale.x;
					parentMat._22 = worldScale.y;
					parentMat._33 = worldScale.z;

					//회전정보만 상속받는 경우: 크기정보만 추출
					//이 코드는 Scale이 음수일시는 작동하지 않는다는 문제점이 있다.
					//float3 Scale(parentMat.Right().Length(), parentMat.Up().Length(), parentMat.Forward().Length());
				}

				//크기 반영 + 회전 반영 -> 그냥 바로 적용
				else
				{
					parentMat *= parent->GetWorldMatrix_Internal();
				}
			}

			m_worldMatrix *= parentMat;
		}

		//월드 방향 계산
		for (int i = 0; i < (int)eDirection::END; ++i)
		{
			m_worldDirection[i] = float3(m_worldMatrix.m[i]).Normalize();
		}


		return true;
	}






	void Com_Transform::SetWorldScale(const float3& _worldScale)
	{
		//부모 스케일 무시 or 부모 트랜스폼 없을 경우
		if (m_bIgnoreParentScale || m_parent.expired())
		{
			SetLocalScale(_worldScale);
		}
		else 
		{
			std::shared_ptr<Com_Transform> parent = m_parent.lock();
			//InternalUpdate 단계가 아직 진행되지 않았을 경우 업데이트 함수 호출.
			//이때 bool 플래그는 바꾸지 않는다.(다른 스크립트에서 아직 위치 업데이트가 끝나지 않았을 수 있음)
			parent->UpdateWorldValue();

			//설정하려는 World Scale을 부모의 World Scale로 나눠 주면 Local Scale을 얼마나 지정해야하는지 알 수 있다.
			SetLocalScale(_worldScale / parent->GetWorldScale_Internal());
		}
	}
	void Com_Transform::SetWorldRotation(const math::Quaternion& _worldRotation)
	{
		//부모 스케일 무시 or 부모 트랜스폼 없을 경우
		if (m_bIgnoreParentRotation || m_parent.expired())
		{
			SetLocalRotation(_worldRotation);
		}
		else
		{
			std::shared_ptr<Com_Transform> parent = m_parent.lock();

			//InternalUpdate 단계가 아직 진행되지 않았을 경우 업데이트 함수 호출.
			//이때 bool 플래그는 바꾸지 않는다.(다른 스크립트에서 아직 위치 업데이트가 끝나지 않았을 수 있음)
			parent->UpdateWorldValue();

			//설정하려는 World Rotation을 부모의 World Rotation로 나눠 주면 Local Rotation을 얼마나 지정해야하는지 알 수 있다.
			SetLocalRotation(_worldRotation / parent->GetWorldRotation_Internal());
		}
	}
	inline void Com_Transform::SetWorldRotation(const float3& _worldRotationEuler)
	{
		SetWorldRotation(Quaternion::CreateFromYawPitchRoll(_worldRotationEuler));
	}
	void Com_Transform::SetWorldPosition(const float3& _worldPosition)
	{
		//부모 트랜스폼 없을 경우
		if (m_parent.expired())
		{
			SetLocalPosition(_worldPosition);
		}
		else
		{
			std::shared_ptr<Com_Transform> parent = m_parent.lock();

			//World Position을 구하기 위해서는 자신 포함 World Matrix 업데이트를 해줘야함(Parent 까지만)
			UpdateWorldMatrix();

			//World Matrix의 위치 정보를 worldPosition으로 설정
			m_worldMatrix.Translation(_worldPosition);

			//UpdateWorldMatrix() 호출 시 Local Matrix, World Matrix 전부 새걸로 갱신되어 있는 상태이므로(flag들이 모두 false 가 되어있는 상태)
			//추가적으로 작업을 안해줘도 됨. -> Set함수를 통하지 않고 변경.
			MATRIX calculatedLocalMatrix = m_worldMatrix * parent->GetWorldMatrix_Internal().Invert();
			m_localPosition = calculatedLocalMatrix.Translation();

			//오차를 최대한 줄이기 위해 position만 가져온다.
			m_localMatrix.Translation(calculatedLocalMatrix.Translation());
		}
	}
}
