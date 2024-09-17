#pragma once
#include "Engine/Common.h"

#include "Engine/Util/StaticSingleton.h"

//전방선언
namespace FMOD
{
	namespace Studio
	{
		class System;
	}
	class Sound;
	class System;
	class Channel;
}

namespace ehw
{
	class AudioManager final : public StaticSingleton<AudioManager>
	{
		friend class StaticSingleton<AudioManager>;

	public:
		bool init();
		void release();

		bool CreateSound(const std::filesystem::path& _fullPath, FMOD::Sound** _sound);
		void SoundPlay(FMOD::Sound* _sound, FMOD::Channel** _channel);
		void Set3DListenerAttributes(const float3* _POS, const float3* _VEL,
			const float3* _forward, const float3* _up);

	protected:
		AudioManager();
		~AudioManager();

	private:
		FMOD::Studio::System* mSystem;
		FMOD::System* mCoreSystem;
	};
}
