#include "PCH_Engine.h"
#include "MeshData.h"

#include "define_Util.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "FBXLoader.h"
#include "GameObject.h"
#include "Com_Renderer_Mesh.h"
#include "Com_Animator3D.h"
#include "define_Util.h"

#include "PathMgr.h"
#include "json-cpp/json.h"
#include "Skeleton.h"

namespace mh
{
	MeshData::MeshData()
		: IRes(define::eResourceType::MeshData)
	{
	}

	MeshData::~MeshData()
	{
	}

	eResult MeshData::Save(const std::filesystem::path& _fileName)
	{
		std::fs::path fullPath = PathMgr::GetContentPathRelative(GetResType());
		if (false == std::fs::exists(fullPath))
			std::fs::create_directories(fullPath);

		fullPath /= _fileName;
		fullPath.replace_extension(strKey::Ext_MeshData);

		std::ofstream ofs(fullPath);
		if (false == ofs.is_open())
			return eResult::Fail_OpenFile;

		Json::Value jVal{};
		eResult result = SaveJson(&jVal);
		if (eResultFail(result))
			return result;
		
		ofs << jVal;
		ofs.close();

		return eResult::Success;
	}

	eResult MeshData::Load(const std::filesystem::path& _filePath)
	{
		std::string ext = StringConv::UpperCaseReturn(_filePath.extension().string());

		//FBX일 경우에는 FBXLoader를 통해서 가져온다.
		if (".FBX" == ext)
		{
			eResult result = LoadFromFBX(_filePath);
			if (eResultFail(result))
				return result;
		}

		//그렇지 않을 경우 json을 통해 로드
		else if (".JSON" == ext)
		{
			std::fs::path fullPath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
			fullPath /= _filePath;
			if (false == std::fs::exists(fullPath.parent_path()))
			{
				if (false == std::fs::create_directories(fullPath))
				{
					ERROR_MESSAGE_W(L"경로 정보가 이상합니다.");
					return eResult::Fail_PathNotExist;
				}
			}

			Json::Value jVal;
			std::ifstream ifs(fullPath);
			if (false == ifs.is_open())
			{
				return eResult::Fail_OpenFile;
			}

			ifs >> jVal;
			ifs.close();
			eResult result = LoadJson(&jVal);
			if (eResultFail(result))
				return result;
		}

		return eResult::Success;
	}

	eResult MeshData::SaveJson(Json::Value* _pJson)
	{
		if (nullptr == _pJson)
			return eResult::Fail_Nullptr;
		eResult result = IRes::SaveJson(_pJson);
		if (eResultFail(result))
			return result;

		//비어있을 경우 저장 불가
		if (mMeshContainers.empty())
		{
			return eResult::Fail_InValid;
		}

		//순회 돌아주면서 array 형태의 json에 append 해준다
		Json::Value& jsonMeshCont = (*_pJson)[JSON_KEY(mMeshContainers)];
		for (size_t i = 0; i < mMeshContainers.size(); ++i)
		{
			Json::Value jsonMeshCont = Json::Value(Json::arrayValue);
			if (nullptr == mMeshContainers[i].pMesh || mMeshContainers[i].pMaterials.empty())
			{
				return eResult::Fail_InValid;
			}

			Json::MH::SaveStrKey(&jsonMeshCont, JSON_KEY(pMesh), mMeshContainers[i].pMesh);
			Json::MH::SaveStrKeyVector(&jsonMeshCont, JSON_KEY(pMaterials), mMeshContainers[i].pMaterials);

			jsonMeshCont.append(jsonMeshCont);
		}

		return eResult::Success;
	}


	eResult MeshData::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
			return eResult::Fail_Nullptr;
		eResult result = IRes::LoadJson(_pJson);
		if (eResultFail(result))
			return result;
		
		mMeshContainers.clear();

		//mesh container 순회 돌아주면서 하나씩 Load
		const Json::Value& jsonMeshCont = (*_pJson)[JSON_KEY(mMeshContainers)];
		for (Json::ValueConstIterator iter = jsonMeshCont.begin();
			iter != jsonMeshCont.end();
			++iter)
		{
			tMeshContainer cont{};

			//Mesh Load
			std::string meshStrKey = Json::MH::LoadStrKey(&jsonMeshCont, JSON_KEY(pMesh), cont.pMesh);
			cont.pMesh = ResMgr::Load<Mesh>(meshStrKey);
			if (nullptr == cont.pMesh)
				return eResult::Fail_Empty;

			//Materials Load
			const auto& materialsStrKey = Json::MH::LoadStrKeyVector(&jsonMeshCont, JSON_KEY(pMaterials), cont.pMaterials);
			for (size_t i = 0; i < materialsStrKey.size(); ++i)
			{
				std::shared_ptr<Material> mtrl = ResMgr::Load<Material>(materialsStrKey[i]);
				cont.pMaterials.push_back(mtrl);
			}

			mMeshContainers.push_back(cont);
		}

		return eResult::Success;
	}


	GameObject* MeshData::Instantiate()
	{
		GameObject* pNewObj = new GameObject;
		Com_Renderer_Mesh* Renderer = pNewObj->AddComponent<Com_Renderer_Mesh>();

		Renderer->SetMesh(mMesh);

		for (UINT i = 0; i < mMaterials.size(); ++i)
		{
			Renderer->SetMaterial(mMaterials[i], i);
		}

		if (mMesh->IsAnimMesh())
		{
			Com_Animator3D* Animator3D = pNewObj->AddComponent<Com_Animator3D>();

			Animator3D->SetBones(mMesh->GetBones());
			Animator3D->SetAnimClip(mMesh->GetAnimClip());
		}

		return pNewObj;
	}

	eResult MeshData::LoadFromFBX(const std::filesystem::path& _fileName)
	{
		std::fs::path fullPath = PathMgr::GetContentPathRelative(eResourceType::MeshData);
		fullPath /= _fileName;
		if (false == std::fs::exists(fullPath.parent_path()))
		{
			if (false == std::fs::create_directories(fullPath))
			{
				ERROR_MESSAGE_W(L"경로 정보가 이상합니다.");
				return eResult::Fail_PathNotExist;
			}
		}

		FBXLoader loader{};
		loader.Init();
		eResult result = loader.LoadFbx(fullPath);
		if (eResultFail(result))
		{
			ERROR_MESSAGE_W(L"FBX 불러오기 실패.");
			return result;
		}

		//컨테이너 갯수만큼 순회를 돌아준다.
		for (int i = 0; i < loader.GetContainerCount(); ++i)
		{
			tMeshContainer meshCont{};

			//컨테이너를 가져옴
			const tFBXContainer* cont = loader.GetContainer(i);

			//예외 처리
			if (nullptr == cont)
			{
				mMeshContainers.clear();
				std::wstring errorMsg = std::to_wstring(i);
				errorMsg += L" 번째 컨테이너가 nullptr 입니다.";
				ERROR_MESSAGE_W(errorMsg.c_str());
				return eResult::Fail_Nullptr;
			}

			//가져올 메쉬를 생성
			meshCont.pMesh = std::make_shared<Mesh>();

			result = meshCont.pMesh->CreateFromContainer(cont);
			if (eResultFail(result))
			{
				ERROR_MESSAGE_W(L"FBX로부터 메쉬 정보를 읽어오는 데 실패했습니다.");
				return eResult::Fail;
			}

			// ResMgr 에 메쉬 등록
			if (nullptr != meshCont.pMesh)
			{
				std::fs::path strKey = _fileName;
				//.msh로 확장자를 변경
				strKey.replace_extension(define::strKey::Ext_Mesh);
				//Key로 Mesh를 저장
				meshCont.pMesh->SetKey(strKey.string());

				//메쉬를 엔진의 포맷으로 변경해서 저장한다.
				eResult result = meshCont.pMesh->Save(strKey);

				if (eResultFail(result))
				{
					ERROR_MESSAGE_W(L"Mesh 저장 실패.");
					return result;
				}


			}

			//std::vector<std::shared_ptr<Material>> vecMtrl;

			// 메테리얼 가져오기
			for (UINT i = 0; i < cont->vecMtrl.size(); ++i)
			{
				//Material의 경우 FBX Loader에서 만들어 놨음
				// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
				std::string strKey = cont->vecMtrl[i].strMtrlName;
				std::shared_ptr<Material> pMtrl = ResMgr::Find<Material>(strKey);

				//혹시나 없을 경우 에러
				MH_ASSERT(pMtrl.get());

				meshCont.pMaterials.push_back(pMtrl);
			}

			//다른게 다 진행됐으면 자신도 저장
			//키값 만들고 세팅하고
			std::fs::path strKeyMeshData = _fileName;
			strKeyMeshData.replace_extension(strKey::Ext_MeshData);
			std::string strKey = strKeyMeshData.string();
			SetKey(strKey);

			//저장함수 호출
			result = Save(strKey);
			if (eResultFail(result))
			{
				return result;
			}

			//전부 저장에 성공했을 경우 ResMgr에서 이 주소(MeshData)를 리소스에 추가한다
			//애초에 호출한 클래스가 ResMgr임
		}

		return eResult::Success;
	}
}

