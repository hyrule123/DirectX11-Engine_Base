#include "Engine/Resource/AudioClip.h"


#include "Engine/Game/Component/Transform.h"

#include "Engine/Manager/ResourceManager.h"

#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace ehw
{
	AudioClip::AudioClip()
		: Resource(AudioClip::concrete_class_name)
		, mSound(nullptr)
		, mChannel(nullptr)
		, mMinDistance(1.0f)
		, mMaxDistance(1000.0f)
		, mbLoop(false)
	{

	}

	AudioClip::~AudioClip()
	{
		mSound->release();
		mSound = nullptr;
	}


	eResult AudioClip::load(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
	{
		std::fs::path fullPath = _base_directory / _resource_name;
		if (false == std::fs::exists(fullPath))
		{
			ERROR_MESSAGE("파일이 없습니다.");
			return eResult::Fail_Open;
		}

		if (false == AudioManager::get_inst().CreateSound(fullPath, &mSound))
			return eResult::Fail_Open;

		mSound->set3DMinMaxDistance(mMinDistance, mMaxDistance);

		return eResult::Success;
	}

	void AudioClip::Play()
	{
		if (mbLoop)
			mSound->setMode(FMOD_LOOP_NORMAL);
		else
			mSound->setMode(FMOD_LOOP_OFF);

		AudioManager::get_inst().SoundPlay(mSound, &mChannel);
	} 

	void AudioClip::Stop()
	{
		mChannel->stop();
	}

	void AudioClip::Set3DAttributes(const float3 _pos, const float3 _vel)
	{
		FMOD_VECTOR fmodPos(_pos.x, _pos.y, _pos.z);
		FMOD_VECTOR fmodVel(_vel.x, _vel.y, _vel.z);

		mChannel->set3DAttributes(&fmodPos, &fmodVel);
	}
}
