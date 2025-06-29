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

namespace core
{
	Com_Animator3D::Com_Animator3D()
		: Animator(Com_Animator3D::s_static_type_name, eDimensionType::_3D)
		, m_sharedPlayData()
	{
	}

	Com_Animator3D::Com_Animator3D(const Com_Animator3D& _other)
		: Animator(_other)
		, m_sharedPlayData()
	{
		if (_other.m_sharedPlayData)
		{
			auto cloned = _other.m_sharedPlayData->Clone();
			m_sharedPlayData = std::static_pointer_cast<Animation3D_PlayData>(cloned);
		}
	}

	Com_Animator3D::~Com_Animator3D()
	{
	}

	eResult Com_Animator3D::serialize_json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult Com_Animator3D::deserialize_json(const JsonSerializer* _ser)
	{
		return eResult();
	}


	void Com_Animator3D::final_update()
	{
		if (nullptr == m_sharedPlayData) { return; }
		else if (false == m_sharedPlayData->is_playing()) { return; }

		if (false == m_sharedPlayData->is_pre_updated()) {
			if (false == m_sharedPlayData->pre_update()) {
				return;
			}
		}

		//이전 프레임 번호 업데이트 및 이벤트함수 호출
		if (m_sharedPlayData->get_prev_frame() != m_sharedPlayData->get_current_frame())
		{
			const s_ptr<Animation3D>& curAnim = m_sharedPlayData->get_current_animation();

			Animator::CallEvent(static_cast<Animation*>(curAnim.get()), m_sharedPlayData->get_current_frame() - curAnim->get_start_frame());
		}
	}

	void Com_Animator3D::frame_end()
	{
		if (m_sharedPlayData)
		{
			m_sharedPlayData->update_prev_frame();
			m_sharedPlayData->frame_end();
		}
	}
	

	bool Com_Animator3D::AddEvent(const std::string_view _animName, uint _frameIdx, const std::function<void()>& _func)
	{
		if (nullptr == m_sharedPlayData)
		{
			return false;
		}

		s_ptr<Skeleton> skeleton = m_sharedPlayData->get_skeleton();
		if (nullptr == m_sharedPlayData->get_skeleton())
		{
			return false;
		}

		s_ptr<Animation3D> anim = skeleton->find_animation(_animName);
		if (nullptr == anim)
		{
			return false;
		}

		//혹시나 frame이 최대 프레임 수보다 많을 경우 최대 프레임 수로 제한한다.
		else if (anim->get_frame_length() <= _frameIdx)
		{
			_frameIdx = anim->get_frame_length() - 1;
		}

		Animator::AddEvent(static_cast<Animation*>(anim.get()), _frameIdx, _func);

		return true;
	}

	s_ptr<Animation3D_PlayData> Com_Animator3D::CreateSharedAnimationData()
	{
		m_sharedPlayData = std::make_shared<Animation3D_PlayData>();

		return m_sharedPlayData;
	}

	bool Com_Animator3D::is_playing() const
	{ 
		return (m_sharedPlayData && m_sharedPlayData->is_playing());
	}



	s_ptr<Skeleton> Com_Animator3D::get_skeleton()
	{
		s_ptr<Skeleton> ret{};
		if (m_sharedPlayData) {
			ret = m_sharedPlayData->get_skeleton();
		}
		return ret;
	}

	bool Com_Animator3D::Play(const std::string_view _strAnimName, float _blendTime)
	{
		if (m_sharedPlayData)
		{
			return m_sharedPlayData->play(_strAnimName, _blendTime);
		}

		return false;
	}

	void Com_Animator3D::PlayNext()
	{
		if (m_sharedPlayData)
		{
			m_sharedPlayData->play_next();
		}
	}

	void Com_Animator3D::bind_computed_final_bone_matrix()
	{
		if (m_sharedPlayData) 
		{
			m_sharedPlayData->bind_computed_final_bone_matrix();
		}
	}

}
