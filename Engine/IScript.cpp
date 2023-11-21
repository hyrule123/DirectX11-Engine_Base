#include "PCH_Engine.h"

#include "IScript.h"



namespace ehw
{
	IScript::IScript()
		: IComponent(define::eComponentType::Scripts)
	{

	}

	IScript::~IScript()
	{
	}
}
