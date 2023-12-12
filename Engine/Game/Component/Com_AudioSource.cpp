#include "../../PCH_Engine.h"
#include "Com_AudioSource.h"



#include "../../Resource/AudioClip.h"

#include "../../Game/GameObject.h"

#include "../../json-cpp/json.h"

#include "../../Manager/ResourceManager.h"


#include "Com_Transform.h"


#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace ehw
{
	Com_AudioSource::Com_AudioSource()
		: iComponent(eComponentCategory::AudioSource)
		, mAudioClip(nullptr)
	{

	}

	eResult Com_AudioSource::SaveJson(Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail;
		}
		eResult result = iComponent::SaveJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}
		Json::Value& jVal = *_pJson;

		if (mAudioClip)
		{
			jVal[JSON_KEY(mAudioClip)] = mAudioClip->GetStrKey();
		}

		return eResult::Success;
	}

	eResult Com_AudioSource::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail;
		}
		eResult result = iComponent::LoadJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}
		const Json::Value& jVal = *_pJson;

		if (jVal.isMember(JSON_KEY(mAudioClip)))
		{
			std::string strKey = jVal[JSON_KEY(mAudilClip)].asString();
			mAudioClip = ResourceManager::Load<AudioClip>(strKey);
		}

		return eResult::Success;
	}

	Com_AudioSource::~Com_AudioSource()
	{

	}


	void Com_AudioSource::InternalUpdate()
	{
		Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
		float3 pos = tr->GetRelativePos();
		float3 foward = tr->Forward();

		mAudioClip->Set3DAttributes(pos, foward);
	}


	void Com_AudioSource::Play()
	{
		mAudioClip->Play();
	}
	void Com_AudioSource::Stop()
	{
		mAudioClip->Stop();
	}
	void Com_AudioSource::SetLoop(bool _loop)
	{
		mAudioClip->SetLoop(_loop);
	}
}
