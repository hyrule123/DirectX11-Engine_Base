#pragma once
#include <Engine/Common.h>

#include <Engine/Util/Singleton.h>

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

namespace core
{
	class AudioManager final
	{
		DECLARE_SINGLETON(AudioManager);
	private:
		AudioManager();
		void init();

		~AudioManager();

	public:
		bool create_sound(const std::filesystem::path& _fullPath, FMOD::Sound** _sound);
		void sound_play(FMOD::Sound* _sound, FMOD::Channel** _channel);
		void set_3D_listener_attrib(const float3* _POS, const float3* _VEL,
			const float3* _forward, const float3* _up);

	private:
		FMOD::System* m_FMOD_core_system;
		FMOD::Studio::System* m_FMOD_system;
	};
}
