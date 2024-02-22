#include "Com_AudioListener.h"


#include "../../../Manager/AudioManager.h"

#include "../../GameObject.h"

#include "../Transform/Com_Transform.h"


#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace ehw
{
	

	Com_AudioListener::Com_AudioListener()
	{

	}

	Com_AudioListener::~Com_AudioListener()
	{

	}

	eResult Com_AudioListener::Serialize_Json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult Com_AudioListener::DeSerialize_Json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void Com_AudioListener::Awake()
	{

	}

	void Com_AudioListener::Update()
	{

	}

	void Com_AudioListener::InternalUpdate()
	{
		const auto& tr = GetOwner()->GetComponent<Com_Transform>();
		float3 pos = tr->();
		float3 foward = tr->Forward();
		float3 up = tr->Up();

		float3 vel = { 0.0f, 0.0f, 0.0f };
		AudioManager::Set3DListenerAttributes(&pos, &vel, &foward, &up);
	}

}
