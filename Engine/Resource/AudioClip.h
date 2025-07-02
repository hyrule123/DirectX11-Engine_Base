#pragma once
#include <Engine/Resource/Resource.h>

#include <Engine/Manager/AudioManager.h>

namespace core
{
	BASE_RESOURCE(AudioClip);
	class AudioClip final : public Resource
	{
		CLASS_INFO(AudioClip, Resource);
	public:
		AudioClip();
		virtual ~AudioClip();

		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;
		
		void Play();
		void Stop();
		void Set3DAttributes(const float3 _pos, const float3 _vel);
		void SetLoop(bool _loop) { mbLoop = _loop; }

	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;
		float mMinDistance;
		float mMaxDistance;
		bool mbLoop;
	};
}

