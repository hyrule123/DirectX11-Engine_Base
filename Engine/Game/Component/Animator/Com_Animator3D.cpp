#include "Com_Animator3D.h"


#include "Engine/Manager/TimeManager.h"
#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/InputManager.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/Resource/Shader/ComputeShaders/Animation3D_ComputeShader.h"
#include "Engine/Resource/Model3D/Skeleton.h"
#include "Engine/Resource/Model3D/Animation3D.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

#include "Engine/Game/Component/Animator/Animation3D_PlayData.h"

namespace ehw
{
	Com_Animator3D::Com_Animator3D()
		: iAnimator(eDimensionType::_3D)
		, m_sharedPlayData()
	{
	}

	Com_Animator3D::Com_Animator3D(const Com_Animator3D& _other)
		: iAnimator(_other)
		, m_sharedPlayData()
	{
		if (_other.m_sharedPlayData)
		{
			m_sharedPlayData = std::shared_ptr<Animation3D_PlayData>(_other.m_sharedPlayData->Clone());
		}
	}

	Com_Animator3D::~Com_Animator3D()
	{
	}

	eResult Com_Animator3D::Serialize_Json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult Com_Animator3D::DeSerialize_Json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void Com_Animator3D::Init()
	{

	}


	void Com_Animator3D::FinalUpdate()
	{
		if (nullptr == m_sharedPlayData)
		{
			return;
		}
		else if (false == m_sharedPlayData->FinalUpdate())
		{
			return;
		}

		//이전 프레임 번호 업데이트 및 이벤트함수 호출
		if (m_sharedPlayData->GetPrevFrame() != m_sharedPlayData->GetCurrentFrame())
		{
			const std::shared_ptr<Animation3D>& curAnim = m_sharedPlayData->GetCurrentAnimation();


			iAnimator::CallEvent(static_cast<iAnimation*>(curAnim.get()), m_sharedPlayData->GetCurrentFrame() - curAnim->GetStartFrame());
		}
	}

	void Com_Animator3D::FrameEnd()
	{
		if (nullptr == m_sharedPlayData)
		{
			return;
		}
		
		m_sharedPlayData->UpdatePrevFrame();
		m_sharedPlayData->FrameEnd();
	}
	

	bool Com_Animator3D::AddEvent(const std::string_view _animName, int _frameIdx, const std::function<void()>& _func)
	{
		if (nullptr == m_sharedPlayData)
		{
			return false;
		}

		std::shared_ptr<Skeleton> skeleton = m_sharedPlayData->GetSkeleton();
		if (nullptr == m_sharedPlayData->GetSkeleton())
		{
			return false;
		}

		std::shared_ptr<Animation3D> anim = skeleton->FindAnimation(_animName);
		if (nullptr == anim)
		{
			return false;
		}

		//혹시나 frame이 최대 프레임 수보다 많을 경우 최대 프레임 수로 제한한다.
		else if (anim->GetFrameLength() <= _frameIdx)
		{
			_frameIdx = anim->GetFrameLength() - 1;
		}

		iAnimator::AddEvent(static_cast<iAnimation*>(anim.get()), _frameIdx, _func);

		return true;
	}

	std::shared_ptr<Animation3D_PlayData> Com_Animator3D::CreateSharedAnimationData()
	{
		m_sharedPlayData = std::make_shared<Animation3D_PlayData>();

		return m_sharedPlayData;
	}

	bool Com_Animator3D::IsPlaying() const
	{ 
		return (m_sharedPlayData && (nullptr != m_sharedPlayData->GetCurrentAnimation()));
	}



	bool Com_Animator3D::Play(const std::string_view _strAnimName, float _blendTime)
	{
		if (m_sharedPlayData)
		{
			return m_sharedPlayData->Play(_strAnimName, _blendTime);
		}

		return false;
	}

	void Com_Animator3D::PlayNext()
	{
		if (m_sharedPlayData)
		{
			m_sharedPlayData->PlayNext();
		}
	}


	void Com_Animator3D::BindData()
	{
		if (m_sharedPlayData)
		{
			m_sharedPlayData->BindData();
		}
	}


	void Com_Animator3D::UnBindData()
	{
		if (m_sharedPlayData)
		{
			m_sharedPlayData->UnBindData();
		}
	}








}
