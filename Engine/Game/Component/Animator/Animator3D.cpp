#include "Animator3D.h"


#include <Engine/Manager/TimeManager.h>
#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/InputManager.h>

#include <Engine/GPU/StructBuffer.h>

#include <Engine/Resource/Shader/ComputeShaders/Animation3D_ComputeShader.h>
#include <Engine/Resource/Model3D/Skeleton.h>
#include <Engine/Resource/Model3D/Animation3D.h>

#include <Engine/Game/GameObject.h>

#include <Engine/Game/Component/Renderer/StaticMeshRenderer.h>

#include <Engine/Game/Component/Animator/Animation3D_PlayData.h>

namespace core
{
	Animator3D::Animator3D()
		: Animator(eDimensionType::_3D)
		, m_shared_play_data()
	{
	}

	Animator3D::Animator3D(const Animator3D& _other)
		: Animator(_other)
		, m_shared_play_data()
	{
		if (_other.m_shared_play_data)
		{
			auto cloned = _other.m_shared_play_data->clone();
			m_shared_play_data = std::static_pointer_cast<Animation3D_PlayData>(cloned);
		}
	}

	Animator3D::~Animator3D()
	{
	}

	void Animator3D::final_update()
	{
		if (nullptr == m_shared_play_data) { return; }
		else if (false == m_shared_play_data->is_playing()) { return; }

		if (false == m_shared_play_data->is_pre_updated()) {
			if (false == m_shared_play_data->pre_update()) {
				return;
			}
		}

		//이전 프레임 번호 업데이트 및 이벤트함수 호출
		if (m_shared_play_data->get_prev_frame() != m_shared_play_data->get_current_frame())
		{
			const s_ptr<Animation3D>& curAnim = m_shared_play_data->get_current_animation();

			Animator::CallEvent(static_cast<Animation*>(curAnim.get()), m_shared_play_data->get_current_frame() - curAnim->get_start_frame());
		}
	}

	void Animator3D::frame_end()
	{
		if (m_shared_play_data)
		{
			m_shared_play_data->update_prev_frame();
			m_shared_play_data->frame_end();
		}
	}
	

	bool Animator3D::add_event(const std::string_view _animName, uint _frameIdx, const std::function<void()>& _func)
	{
		if (nullptr == m_shared_play_data)
		{
			return false;
		}

		s_ptr<Skeleton> skeleton = m_shared_play_data->get_skeleton();
		if (nullptr == m_shared_play_data->get_skeleton())
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

		Animator::add_event(static_cast<Animation*>(anim.get()), _frameIdx, _func);

		return true;
	}

	s_ptr<Animation3D_PlayData> Animator3D::create_shared_animation_data()
	{
		m_shared_play_data = new_entity<Animation3D_PlayData>();

		return m_shared_play_data;
	}

	bool Animator3D::is_playing() const
	{ 
		return (m_shared_play_data && m_shared_play_data->is_playing());
	}



	s_ptr<Skeleton> Animator3D::get_skeleton()
	{
		s_ptr<Skeleton> ret{};
		if (m_shared_play_data) {
			ret = m_shared_play_data->get_skeleton();
		}
		return ret;
	}

	bool Animator3D::play(const std::string_view _strAnimName, float _blendTime)
	{
		if (m_shared_play_data)
		{
			return m_shared_play_data->play(_strAnimName, _blendTime);
		}

		return false;
	}

	void Animator3D::play_next_animation()
	{
		if (m_shared_play_data)
		{
			m_shared_play_data->play_next();
		}
	}

	void Animator3D::bind_computed_final_bone_matrix()
	{
		if (m_shared_play_data) 
		{
			m_shared_play_data->bind_computed_final_bone_matrix();
		}
	}

}
