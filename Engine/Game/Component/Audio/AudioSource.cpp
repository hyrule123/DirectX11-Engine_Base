#include "AudioSource.h"

#include <Engine/Resource/AudioClip.h>
#include <Engine/Manager/ResourceManager.h>

#include <Engine/Game/GameObject.h>

#include <Engine/Game/Component/Transform.h>

#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace core
{
	AudioSource::AudioSource()
		: Super(AudioSource::s_static_type_name, s_component_order)
		, m_audio_clip(nullptr)
	{
	}

	//eResult AudioSource::SaveJson(Json::Value* _pJson)
	//{
	//	if (nullptr == _pJson)
	//	{
	//		return eResult::Fail;
	//	}
	//	eResult result = Component::SaveJson(_pJson);
	//	if (eResult_fail(result))
	//	{
	//		return result;
	//	}
	//	Json::Value& jVal = *_pJson;

	//	if (m_audio_clip)
	//	{
	//		jVal[JSON_KEY(m_audio_clip)] = m_audio_clip->get_static_type_name();
	//	}

	//	return eResult::Success;
	//}

	//eResult AudioSource::LoadJson(const Json::Value* _pJson)
	//{
	//	if (nullptr == _pJson)
	//	{
	//		return eResult::Fail;
	//	}
	//	eResult result = Component::LoadJson(_pJson);
	//	if (eResult_fail(result))
	//	{
	//		return result;
	//	}
	//	const Json::Value& jVal = *_pJson;

	//	if (jVal.isMember(JSON_KEY(m_audio_clip)))
	//	{
	//		std::string name = jVal[JSON_KEY(mAudilClip)].asString();
	//		m_audio_clip = ResourceManager<AudioClip>::get_inst().Load(name);
	//	}

	//	return eResult::Success;
	//}

	AudioSource::~AudioSource()
	{

	}

	eResult AudioSource::serialize_json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult AudioSource::deserialize_json(const JsonSerializer* _ser)
	{
		return eResult();
	}


	void AudioSource::final_update()
	{
		const auto& tr = get_owner()->get_component<Transform>();
		float3 pos = tr->get_local_position();
		float3 foward = tr->get_local_direction(eDirection::Forward);

		m_audio_clip->set_3D_attributes(pos, foward);
	}


	void AudioSource::play()
	{
		m_audio_clip->play();
	}
	void AudioSource::stop()
	{
		m_audio_clip->stop();
	}
	void AudioSource::set_loop(bool _loop)
	{
		m_audio_clip->set_loop(_loop);
	}
}
