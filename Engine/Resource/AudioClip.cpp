#include "AudioClip.h"


#include "../Game/Component/Transform/Com_Transform.h"

#include "../Manager/PathManager.h"


#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace ehw
{
	AudioClip::AudioClip()
		: iResource(eResourceType::AudioClip)
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


	eResult AudioClip::Load(const std::fs::path& _filePath)
	{
		iResource::Save(_filePath);

		std::fs::path fullPath = PathManager::CreateFullPathToContent(_filePath, GetResType());
		if (false == std::fs::exists(fullPath))
		{
			ERROR_MESSAGE_W(L"파일이 없습니다.");
			return eResult::Fail_Open;
		}

		if (false == AudioManager::CreateSound(fullPath, &mSound))
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

		AudioManager::SoundPlay(mSound, &mChannel);
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
