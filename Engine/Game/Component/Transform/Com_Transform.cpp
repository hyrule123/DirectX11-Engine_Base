#include "Com_Transform.h"



#include "../../../Game/GameObject.h"
#include "../../../Manager/RenderManager.h"

#include "../../../GPU/ConstBuffer.h"
#include "../../../Application.h"


#include "../Camera/Com_Camera.h"



namespace ehw
{
	Com_Transform::Com_Transform()
		: m_localScale(float3::One)
		, m_localSize(float3::One)
		, m_localRotation()
		, m_localPosition()
		, m_localDirection{}
		, m_localMatrix(MATRIX::Identity)
		, m_worldScale(float3::One)
		, m_worldRotation()
		, m_worldDirection{}
		, m_worldMatrix(MATRIX::Identity)
		, m_bNeedUpdateLocal(true)
		, m_bNeedUpdateChild(true)
		, m_bInternalUpdated(false)

		, m_parent()
		, m_childs()
	{
		float3 m_worldScale;

		//월드 회전(참고 - 월드 회전은 부모 트랜스폼으로부터 누적시켜서 '비교를 위해서만' 사용한다.
		math::Quaternion m_worldRotation;

		//월드 위치는 구할 수 없음.

		float3 m_worldDirection[(int)eDirectionType::END];
	}

	Com_Transform::~Com_Transform()
	{
	}

	void Com_Transform::InternalUpdate()
	{
		//이번 프레임에 이미 update 되었을 경우 return
		if (m_bInternalUpdated)
		{
			return;
		}
		else
		{
			m_bInternalUpdated = true;
		}

		InternalUpdate_Impl(false);
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

	void Com_Transform::UpdateLocalMatrix()
	{
		//1. 크기행렬
		m_localMatrix = MATRIX::CreateScale(m_localScale);

		//2. 회전행렬
		MATRIX matRot = MATRIX::CreateFromQuaternion(m_localRotation);
		m_localMatrix *= matRot;

		//2-1. 회전행렬으로부터 직관적 방향을 계산한다.
		m_localDirection[(int)eDirectionType::FRONT] = matRot.Forward();
		m_localDirection[(int)eDirectionType::RIGHT] = matRot.Right();
		m_localDirection[(int)eDirectionType::UP] = matRot.Up();

		//3. 이동행렬
		m_localMatrix *= MATRIX::CreateTranslation(m_localPosition);
	}

	void Com_Transform::UpdateWorldMatrix()
	{
		MATRIX parentMat = MATRIX::Identity;
		m_worldScale = m_localScale;
		m_worldRotation = m_localRotation;
		m_worldMatrix = m_localMatrix;

		std::shared_ptr<Com_Transform> parent = m_parent.lock();

		constexpr size_t rowErasebytes = sizeof(float) * 3;

		if (parent)
		{
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
					//부모의 World Scale의 역수를 만들어서 월드행렬 앞에 곱해준다(회전정보만 제거)
					parentMat = MATRIX::CreateScale(float3(1.f) / parent->GetWorldScale());
					parentMat *= parent->GetWorldMatrix();

					m_worldRotation *= parent->GetWorldRotation();
					//정규화해서 크기정보를 제거
					//parentMat.Right(parentMat.Right().Normalize());
					//parentMat.Up(parentMat.Up().Normalize());
					//parentMat.Forward(parentMat.Forward().Normalize());
				}
			}

			//크기 반영
			else
			{
				m_worldScale *= parent->GetWorldScale();

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
			}
		}

		
		m_worldMatrix *= parentMat;

		//월드 방향 계산
		for (int i = 0; i < (int)eDirectionType::END; ++i)
		{
			m_worldDirection[i] = float3(m_worldMatrix.m[i]);
		}
	}

	void Com_Transform::InternalUpdate_Impl(bool _bSilencedUpdate)
	{
		bool bNeedUpdateWorldMatrix = false;

		//Local Matrix Update
		if (m_bNeedUpdateLocal)
		{
			UpdateLocalMatrix();

			//로컬 변수 변경내용이 반영되었으므로 false
			m_bNeedUpdateLocal = false;
			bNeedUpdateWorldMatrix = true;
		}


		//Parent 있을 경우 업데이트 되었는지 확인
		const std::shared_ptr<Com_Transform>& parent = GetParent();
		if (parent && parent->IsNeedUpdateChild())
		{
			//만약 InternalUpdate 단계까지 진행되지 않은 상태일 경우 InternalUpdate 함수를 호출한다.
			if (false == parent->IsInlineUpdated())
			{
				//강제 업데이트일 경우 impl함수를 직접 호출
				if (_bSilencedUpdate)
				{
					parent->InternalUpdate_Impl(_bSilencedUpdate);
				}
				else
				{
					parent->InternalUpdate();
				}
			}

			bNeedUpdateWorldMatrix = true;
		}

		//자신 or 부모 둘중 하나라도 업데이트 되었을 경우 World Matrix 업데이트 해준다
		if (bNeedUpdateWorldMatrix)
		{
			UpdateWorldMatrix();
		}
	}



	void Com_Transform::SetWorldScale(const float3& _localScale)
	{
		//InternalUpdate 단계가 아직 진행되지 않았을 경우 업데이트 함수 호출.
		//이때 bool 플래그는 바꾸지 않는다.(다른 스크립트에서 아직 위치 업데이트가 끝나지 않았을 수 있음)
		if (false == m_bInternalUpdated)
		{
			InternalUpdate_Impl(true);
		}

		//Scale 값 추출
		float3 worldScale{};
		math::Quaternion worldRotation{};
		float3 worldPos{};

		m_worldMatrix.Decompose(worldScale, worldRotation, worldPos);

	}
	void Com_Transform::SetWorldRotation(const math::Quaternion& _quat)
	{
	}
	void Com_Transform::SetWorldPosition(const float3& _localPosition)
	{
	}
}
