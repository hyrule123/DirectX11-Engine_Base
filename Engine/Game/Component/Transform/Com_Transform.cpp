#include "Com_Transform.h"



#include "../../../Game/GameObject.h"
#include "../../../Manager/RenderManager.h"

#include "../../../GPU/ConstBuffer.h"
#include "../../../Application.h"


#include "../Camera/Com_Camera.h"


namespace ehw
{
	Com_Transform::Com_Transform()
		: mSize(1.f, 1.f, 1.f)
		, mScaleRelative(1.f, 1.f, 1.f)
		, mPosRelative{}
		, mRotRelative{}
		, mDirRelative{ float3::UnitX, float3::UnitY, float3::UnitZ }
		, mDirWorld{ float3::UnitX, float3::UnitY, float3::UnitZ }
		, mMatRelative{ MATRIX::Identity }
		, mMatParent{ MATRIX::Identity }
		, mCB_Transform{}
		, mbInheritScale(true)
		, mbInheritRot(true)
		, mbLockRotation(false)
		, mbSizeUpdated(true)
		, mbNeedMyUpdate(true)
	{
	}

	Com_Transform::~Com_Transform()
	{
	}

	void Com_Transform::InternalUpdate()
	{
		//bool 값들은 Tick()에서 false로 초기화 된다.
		//여기선 고유 크기(Size)를 반영하지 않은 월드행렬을 만든다.
		//게임오브젝트 상속 관계에서 고유 크기까지 상속을 받게 되면 기하급수적으로 크기가 커짐 
		if (true == mbNeedMyUpdate)
		{
			//자신의 트랜스폼 업데이트를 진행할 경우 - 두개 다 업데이트 해줘야함.
			UpdateMyTransform();
		}

		//부모 트랜스폼이 갱신되었는지 확인하고, 갱신되었을 경우 자신의 행렬도 갱신
		const auto& parent = GetOwner()->GetParent();
		if (parent)
		{
			const auto& tr = parent->GetComponent<Com_Transform>();
			if (tr && tr->IsUpdated())
			{
				UpdateParentMatrix(tr);
				mbNeedMyUpdate = true;
			}
		}
		

		//둘중에 하나라도 업데이트 되었을 경우 월드행렬을 새로 계산한다.
		if (mbNeedMyUpdate || mbSizeUpdated)
		{
			//부모 트랜스폼이 없을 경우 mMatparent에는 단위행렬이 들어있음
			mCB_Transform.World = mMatParent;

			//여기에 자신의 사이즈를 곱해줌
			mCB_Transform.World *= MATRIX::CreateScale(mSize);

			//마지막으로 Relative Matrix를 곱해주면 끝
			mCB_Transform.World *= mMatRelative;
			mCB_Transform.InverseWorld = mCB_Transform.World.Invert();
		}

		//Application::(std::bind(&Com_Transform::ClearUpdateState, this));
	}

	//eResult Com_Transform::SaveJson(Json::Value* _pJson)
	//{
	//	return eResult::Fail_NotImplemented;

	//	if (nullptr == _pJson)
	//		return eResult::Fail_Nullptr;

	//	//Com_Transform 항목을 하나 만들어서 그 안에 저장
	//	Json::Value& jVal = (*_pJson);

	//	//{//사이즈 X, Y, Z 순서로 저장
	//	//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3Size);
	//	//	jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
	//	//	jVal[strKey].append(Pack_float_int(mSize.x).i);
	//	//	jVal[strKey].append(Pack_float_int(mSize.y).i);
	//	//	jVal[strKey].append(Pack_float_int(mSize.z).i);
	//	//}

	//	//{
	//	//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeScale);
	//	//	jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
	//	//	jVal[strKey].append(Pack_float_int(mScaleRelative.x).i);
	//	//	jVal[strKey].append(Pack_float_int(mScaleRelative.y).i);
	//	//	jVal[strKey].append(Pack_float_int(mScaleRelative.z).i);
	//	//}

	//	//{
	//	//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativePos);
	//	//	jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
	//	//	jVal[strKey].append(Pack_float_int(m_v3RelativePos.x).i);
	//	//	jVal[strKey].append(Pack_float_int(m_v3RelativePos.y).i);
	//	//	jVal[strKey].append(Pack_float_int(m_v3RelativePos.z).i);
	//	//}

	//	//{
	//	//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeRot);
	//	//	jVal[strKey] = Json::Value(Json::ValueType::arrayValue);
	//	//	jVal[strKey].append(Pack_float_int(m_v3RelativeRot.x).i);
	//	//	jVal[strKey].append(Pack_float_int(m_v3RelativeRot.y).i);
	//	//	jVal[strKey].append(Pack_float_int(m_v3RelativeRot.z).i);
	//	//}

	//	//{
	//	//	jVal[std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritRot)] = mbInheritRot;
	//	//	jVal[std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritScale)] = mbInheritScale;
	//	//	jVal[std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bLockRot)] = mbLockRotation;
	//	//}

	//	return eResult::Success;
	//}

	//eResult Com_Transform::LoadJson(const Json::Value* _pJson)
	//{
	//	return eResult::Fail_NotImplemented;

	//	if (nullptr == _pJson)
	//		return eResult::Fail_Nullptr;

	//	//Json::Value& jVal = (*_pJson);

	//	//auto ReadVec3FromJson = [](const Json::Value& _jVal, const std::string& _strKey, float3& _outVal)->bool
	//	//	{
	//	//		if (_jVal.isMember(_strKey))
	//	//		{
	//	//			if (_jVal[_strKey].size() != 3)
	//	//				return false;

	//	//			_outVal.x = Pack_float_int(_jVal[_strKey][0].asInt()).f;
	//	//			_outVal.y = Pack_float_int(_jVal[_strKey][1].asInt()).f;
	//	//			_outVal.z = Pack_float_int(_jVal[_strKey][2].asInt()).f;

	//	//			return true;
	//	//		}
	//	//		else
	//	//			return false;
	//	//	};



	//	//{//RelativeSize
	//	//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3Size);
	//	//	float3 size;
	//	//	if (ReadVec3FromJson(jVal, strKey, size))
	//	//	{
	//	//		SetSize(size);
	//	//	}
	//	//	else
	//	//		return false;
	//	//}


	//	//{//RelativeScale
	//	//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeScale);
	//	//	float3 scale;
	//	//	if (ReadVec3FromJson(jVal, strKey, scale))
	//	//	{
	//	//		SetRelativeScale(scale);
	//	//	}
	//	//	else
	//	//		return false;
	//	//}

	//	//{//RelativePos
	//	//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativePos);
	//	//	float3 pos;
	//	//	if (ReadVec3FromJson(jVal, strKey, pos))
	//	//	{
	//	//		SetRelativePos(pos);
	//	//	}
	//	//	else
	//	//		return false;
	//	//}

	//	//{//RelativeRot
	//	//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::v3RelativeRot);
	//	//	float3 rot;
	//	//	if (ReadVec3FromJson(jVal, strKey, rot))
	//	//	{
	//	//		SetRelativeRot(rot);
	//	//	}
	//	//	else
	//	//		return false;
	//	//}


	//	//{
	//	//	std::string strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritRot);
	//	//	if (jVal.isMember(strKey))
	//	//		mbInheritRot = jVal[strKey].asBool();
	//	//	else return false;

	//	//	strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bInheritScale);
	//	//	if (jVal.isMember(strKey))
	//	//		mbInheritScale = jVal[strKey].asBool();
	//	//	else return false;

	//	//	strKey = std::string(RES_INFO::PREFAB::COMPONENT::TRANSFORM::bLockRot);
	//	//	if (jVal.isMember(strKey))
	//	//	{
	//	//		mbLockRotation = jVal[strKey].asBool();
	//	//	}
	//	//	else return false;
	//	//}


	//	return eResult::Success;
	//}



	void Com_Transform::UpdateMyTransform()
	{
		//1. 크기행렬
		mMatRelative = MATRIX::CreateScale(mScaleRelative);

		//2. 회전행렬
		MATRIX matRot = MATRIX::CreateFromQuaternion(math::Quaternion::CreateFromYawPitchRoll(mRotRelative.y, mRotRelative.x, mRotRelative.z));
		//회전행렬으로부터 직관적 방향을 계산한다.
		mDirRelative[(UINT)eDirectionType::FRONT] = matRot.Forward();
		mDirRelative[(UINT)eDirectionType::RIGHT] = matRot.Right();
		mDirRelative[(UINT)eDirectionType::UP] = matRot.Up();
		//방향은 쿼터니언을 사용해서 계산.

		//회전 잠금 상태가 아닐 경우에만 회전행렬을 곱해준다.
		if (false == mbLockRotation)
			mMatRelative *= matRot;

		//3. 이동행렬
		mMatRelative *= MATRIX::CreateTranslation(mPosRelative);
	}


	void Com_Transform::UpdateParentMatrix(const std::shared_ptr<Com_Transform>& _parentTransform)
	{
		mMatParent = MATRIX::Identity;

		if (_parentTransform)
		{
			mMatParent = _parentTransform->GetWorldMat();

			//부모 오브젝트가 있을 경우 부모의 월드행렬을 받아온다. 
			//성공 시 true가 반환되므로 이 때는 상속 과정을 시작하면 됨
			bool bWorldDirInherit = false;
			if (true == mbInheritRot)
			{
				//회전 상속 + 크기 미상속 -> 크기정보 제거
				if (false == mbInheritScale)
				{
					//정규화해서 크기정보를 제거
					mMatParent.Right(mMatParent.Right().Normalize());
					mMatParent.Up(mMatParent.Up().Normalize());
					mMatParent.Forward(mMatParent.Forward().Normalize());
				}
				//else: 둘 다 상속 받는 경우에는 작업할 것이 없음. 그냥 빠져나가면 됨

				bWorldDirInherit = true;	//이때만 월드방향을 상속받아주면 된다.
			}
			else
			{
				constexpr size_t eraseSize = sizeof(float) * 12;
				//회전 미상속 + 크기 상속 -> 회전정보 제거
				if (true == mbInheritScale)
				{
					//회전정보만 상속받는 경우: 크기정보만 추출
					float3 Scale(mMatParent.Right().Length(), mMatParent.Up().Length(), mMatParent.Forward().Length());
					//float(4) * 12 -> 회전 파트를 모두 0으로 밀어버리고 크기만 등록

					memset(mMatParent.m, 0, eraseSize);
					mMatParent._11 = Scale.x;
					mMatParent._22 = Scale.y;
					mMatParent._33 = Scale.z;
				}
				//회전 미상속 + 크기 미상속 -> 전부 밀고 단위행렬로
				else
				{
					memset(mMatParent.m, 0, eraseSize);
					mMatParent._11 = 1.f;
					mMatParent._22 = 1.f;
					mMatParent._33 = 1.f;
				}
			}
		}
	}

	void Com_Transform::BindData()
	{
		mCB_Transform.View = Com_Camera::GetGpuViewMatrix();
		mCB_Transform.InverseView = Com_Camera::GetGpuViewInvMatrix();
		mCB_Transform.Projection = Com_Camera::GetGpuProjectionMatrix();
		mCB_Transform.WorldView = mCB_Transform.World * mCB_Transform.View;
		mCB_Transform.WVP = mCB_Transform.WorldView * mCB_Transform.Projection;

		ConstBuffer* cb = RenderManager::GetConstBuffer(eCBType::Transform);
		cb->SetData(&mCB_Transform);
		cb->BindData(eShaderStageFlag::ALL);
	}
}
