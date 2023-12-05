#pragma once
#include "iScene.h"
#include "Layer.h"
#include "GameObject.h"
#include "SceneMgr.h"
#include "Com_Transform.h"

namespace ehw::object
{
	//inline GameObject* Instantiate(eLayerType _type, GameObject* _pObj)
	//{
	//	iScene* scene = SceneMgr::GetActiveScene();

	//	_pObj->AddToLayerRecursive(_type);

	//	if(scene->IsInitialized())
	//		_pObj->Init();

	//	return _pObj;
	//}


	//static void Destroy(GameObject* _gameObject)
	//{
	//	_gameObject->Death();
	//}

	//static void DontDestroyOnLoad(GameObject* _gameObject)   //씬 이동시 이 오브젝트는 삭제하지 않는다
	//{
	//	if (_gameObject == nullptr)
	//		return;

	//	_gameObject->DontDestroyOnSceneChange(true);
	//}
}
