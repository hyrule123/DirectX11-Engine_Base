#include "Engine/Resource/Model3D/FBXLoader.h"

#include "Engine/Manager/PathManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Texture.h"
#include "Engine/Resource/Material/Material.h"
#include "Engine/Resource/Shader/GraphicsShader.h"
#include "Engine/Resource/Model3D/Model3D.h"

#include "Engine/Util/define_Util.h"


namespace ehw
{
	constexpr const char* strMixamo = "mixamo.com";
	

	FBXLoader::FBXLoader()
		: Entity(FBXLoader::concrete_class_name)
		, mManager()
		, mScene()
		, mBones{}
		, mAnimNames{}
		, mAnimClips{}
		, mbMixamo{}
		, mFileName{}
	{
	}

	FBXLoader::~FBXLoader()
	{
		Reset();
	}

	void FBXLoader::Reset()
	{
		mContainers.clear();
		mBones.clear();

		for (int i = 0; i < mAnimNames.Size(); ++i)
		{
			if (mAnimNames[i])
			{
				mAnimNames[i]->Clear();
				delete mAnimNames[i];
			}
		}
		mAnimNames.Clear();

		mAnimClips.clear();
		mbMixamo = false;

		if (mScene)
		{
			mScene->Destroy();
			mScene = nullptr;
		}
		if (mManager)
		{
			mManager->Destroy();
			mManager = nullptr;
		}
	}

	eResult FBXLoader::load_FBX(const std::fs::path& _strPath, bool _bStatic)
	{
		Reset();

		if (false == std::fs::exists(_strPath))
		{
			return eResult::Fail_Open;
		}
		
		mManager = fbxsdk::FbxManager::Create();
		if (nullptr == mManager)
		{
			ASSERT(mManager, "fbx Manager가 nullptr 입니다.");
			Reset();
			return eResult::Fail_Nullptr;
		}
		
		mManager->SetIOSettings(fbxsdk::FbxIOSettings::Create(mManager, IOSROOT));

		mScene = fbxsdk::FbxScene::Create(mManager, "");
		if (nullptr == mScene)
		{
			ASSERT(mManager, "fbx Scene이 nullptr 입니다.");
			Reset();
			return eResult::Fail_Nullptr;
		}

		fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(mManager, "");

		if (false == importer->Initialize(_strPath.string().c_str(), -1, mManager->GetIOSettings()))
		{
			ERROR_MESSAGE("FBX 로더 초기화 실패");
			Reset();
			if (importer)
			{
				importer->Destroy();
			}
			return eResult::Fail_Open;
		}

		if (false == importer->Import(mScene))
		{
			ERROR_MESSAGE("임포트 실패.");
			return eResult::Fail;
		}

		mFileName = _strPath.stem().string();


		//축 정보 변경
		//이전 코드
		//fbxsdk::FbxAxisSystem originAxis = fbxsdk::FbxAxisSystem::eMax;
		//originAxis = mScene->GetGlobalSettings().GetAxisSystem();
		//fbxsdk::FbxAxisSystem DesireAxis = fbxsdk::FbxAxisSystem::DirectX;
		//DesireAxis.ConvertScene(mScene);
		//originAxis = mScene->GetGlobalSettings().GetAxisSystem();

		//새로운 코드
		if (mScene->GetGlobalSettings().GetAxisSystem() != fbxsdk::FbxAxisSystem::Max) {
			mScene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::Max);
		}	

		//전체 노드 갯수를 구해서 vector 크기를 예약 시켜놓는다.(이게 최대 수치임)
		int childCount = GetAllNodesCountRecursive(mScene->GetRootNode()) + 1;
		mContainers.reserve(childCount);

		//static 메쉬로 불러오기 설정이 아닐 경우
		if (false == _bStatic)
		{
			//애니메이션 이름을 쭉 받아온다.
			mScene->FillAnimStackNameArray(mAnimNames);

			if (0 < mAnimNames.GetCount())
			{
				load_animation_clip();

				// 본 정보를 읽어온다.
				mBones.reserve(childCount);
				load_skeleton(mScene->GetRootNode());

				// 클립이 가지고 있는 프레임을 본 개수만큼 resize 해준다.
				// 원래 Animation Clip에서 하던것이다.
				for (size_t i = 0; i < mAnimClips.size(); ++i)
				{
					mAnimClips[i].KeyFramesPerBone.resize(mBones.size());
				}
			}
		}

		// 삼각화(triangulate)
		triangulate(mScene->GetRootNode());

		// 메쉬 데이터 얻기
		convert_from_FBX_tree(mScene->GetRootNode(), _bStatic);

		importer->Destroy();
		importer = nullptr;

		// 필요한 텍스쳐 로드
		//load_textures();

		// 필요한 메테리얼 생성
		//CreateMaterial(_strPath);

		return eResult::Success;
	}

	MATRIX FBXLoader::get_matrix_from_FbxAMatrix(const fbxsdk::FbxAMatrix& _mat)
	{
		MATRIX mat;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				mat.m[i][j] = (float)_mat.Get(i, j);
			}
		}
		return mat;
	}



	void FBXLoader::convert_from_FBX_tree(fbxsdk::FbxNode* _pNode, bool _bStatic)
	{
		// 노드의 메쉬정보 읽기
		fbxsdk::FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

		if (pAttr && pAttr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
		{
			//fbxsdk::FbxAMatrix matGlobal = _pNode->EvaluateGlobalTransform();
			//matGlobal.GetR();
			fbxsdk::FbxMesh* mesh = _pNode->GetMesh();
			if (mesh) {
				mContainers.push_back(load_container(mesh, _bStatic));
				if (mContainers.back().Name.empty()) {
					mContainers.back().Name = mFileName + "_" + std::to_string(mContainers.size()); 
				}
			}
		}

		// 자식 노드 정보 읽기
		int iChildCnt = _pNode->GetChildCount();
		for (int i = 0; i < iChildCnt; ++i)
		{
			convert_from_FBX_tree(_pNode->GetChild(i), _bStatic);
		}
	}

	tFBXContainer FBXLoader::load_container(fbxsdk::FbxMesh* _pFBXMesh, bool _bStatic)
	{
		tFBXContainer cont{};// = mContainers.back();

		cont.Name = _pFBXMesh->GetName();

		// ControlPoint 는 위치정보를 담고 있는 배열이다.
		// 이 배열의 개수는 곧 정점의 개수가 된다.
		// 정점에 대응되는 요소들을 Resize 해준다
		int iVtxCnt = _pFBXMesh->GetControlPointsCount();
		cont.resize_by_vertices_count(iVtxCnt);

		// 내부적으로 fbxsdk::FbxVector4타입의 배열로 저장하고 있기 때문에 배열의 
		// 시작주소를 얻어온다.
		fbxsdk::FbxVector4* pFbxPos = _pFBXMesh->GetControlPoints();
		for (int i = 0; i < iVtxCnt; ++i)
		{
			// y와 z축이 바뀌어 있으므로 변경해준다.
			cont.vecPosition[i].x = (float)pFbxPos[i].mData[0];
			cont.vecPosition[i].y = (float)pFbxPos[i].mData[2];
			cont.vecPosition[i].z = (float)pFbxPos[i].mData[1];
			cont.vecPosition[i].w = (float)pFbxPos[i].mData[3];
		}

		// 해당 노드의 재질정보 읽기
		// mesh의 부분별 재질을 통해 Index Buffer를 나눠줄 예정 ( ==> SubSet 개수 ==> Index Buffer Count)
		int materialCount = _pFBXMesh->GetNode()->GetMaterialCount();
		if (0 == materialCount)
		{
			materialCount = 1;
		}

		cont.vecIndexBuffers.resize(materialCount);
		for (int i = 0; i < materialCount; ++i)
		{
			fbxsdk::FbxSurfaceMaterial* pMtrlSur = _pFBXMesh->GetNode()->GetMaterial(i);
			cont.vecMtrl.push_back(load_material(pMtrlSur));

			//무조건 이름을 가지고있게 해준다
			if (cont.vecMtrl.back().strMtrlName.empty())
			{
				size_t contIdx = mContainers.size();
				size_t mtrlIdx = cont.vecMtrl.size();
				cont.vecMtrl.back().strMtrlName = mFileName;
				cont.vecMtrl.back().strMtrlName = "_";
				cont.vecMtrl.back().strMtrlName += std::to_string(contIdx);
				cont.vecMtrl.back().strMtrlName += "_";
				cont.vecMtrl.back().strMtrlName += std::to_string(mtrlIdx);
			}
		}

		// 정점 정보가 속한 subset 을 알기위해서 재질 정보를 얻어온다.
		//(지금 넣을 값이 몇번 index 버퍼에 들어가는지)
		//contains information about how the geometry is associated with materials
		fbxsdk::FbxGeometryElementMaterial* material = _pFBXMesh->GetElementMaterial();
		int vertexOrder = 0;

		// 폴리곤 개수
		int polygonCount = _pFBXMesh->GetPolygonCount();

		//삼각형 수만큼 반복한다.
		for (int i = 0; i < polygonCount; ++i)
		{
			//이 폴리곤을 구성하는 정점의 수를 얻어온다.
			//삼각화를 진행한 상태이므로 무조건 3개가 들어와야 한다.
			int polygonSize = _pFBXMesh->GetPolygonSize(i);
			ASSERT(3 == polygonSize, "삼각화를 진행했는데 1개 폴리곤의 정점이 3개가 아닙니다.");

			//폴리곤 1개가 실제 몇번째 정점을 가르키는지를 저장
			int vertexIndexOfPolygon[3] = {};
			for (int j = 0; j < polygonSize; ++j)
			{
				//현재 삼각형을 구성하고 있는 버텍스정보 내에서의 인덱스를 구한다.(->해당 정점이 실제 몇번째 인덱스인지)
				int vertexIdx = _pFBXMesh->GetPolygonVertex(i, j);
				ASSERT(0 <= vertexIdx, "해당 버텍스의 인덱스 번호가 음수입니다.");
				vertexIndexOfPolygon[j] = vertexIdx;

				cont.vecTangent[vertexIdx] 
					= get_tangent(_pFBXMesh, vertexIdx, vertexOrder);
				cont.vecBinormal[vertexIdx] 
					= get_binormal(_pFBXMesh, vertexIdx, vertexOrder);
				cont.vecNormal[vertexIdx]
					= get_normal(_pFBXMesh, vertexIdx, vertexOrder);

				//TBN중 0이 아닌 값이 있을 경우 bump texture
				if (cont.vecTangent[vertexIdx] != float3::Zero 
					|| cont.vecBinormal[vertexIdx] != float3::Zero 
					|| cont.vecNormal[vertexIdx] != float3::Zero) {
					cont.bBump = true;
				}

				int uvIndex = _pFBXMesh->GetTextureUVIndex(i, j);
				cont.vecUV[vertexIdx] = get_UV(_pFBXMesh, vertexIdx, uvIndex);

				++vertexOrder;
			}

			//material별로 index 버퍼를 나누기로 했으므로
			//삼각형이 소속된 서브셋 인덱스를 얻어온다.
			//만약 없을 경우 0번에 다 때려박는다(하나로 뭉쳐준다)
			int	subsetIndex = 0;
			if (material)
			{
				subsetIndex = material->GetIndexArray().GetAt(i);
			}

			cont.vecIndexBuffers[subsetIndex].push_back(vertexIndexOfPolygon[0]);
			cont.vecIndexBuffers[subsetIndex].push_back(vertexIndexOfPolygon[2]);
			cont.vecIndexBuffers[subsetIndex].push_back(vertexIndexOfPolygon[1]);
		}

		if (false == _bStatic)
		{
			load_animation_data(cont, _pFBXMesh);
		}

		return cont;
	}

	tFBXMaterial FBXLoader::load_material(fbxsdk::FbxSurfaceMaterial* _pMtrlSur)
	{
		tFBXMaterial tMtrlInfo{};

		tMtrlInfo.strMtrlName = _pMtrlSur->GetName();

		// Diff
		tMtrlInfo.DiffuseColor = get_material_data(_pMtrlSur
			, fbxsdk::FbxSurfaceMaterial::sDiffuse
			, fbxsdk::FbxSurfaceMaterial::sDiffuseFactor);

		// Amb
		tMtrlInfo.AmbientColor = get_material_data(_pMtrlSur
			, fbxsdk::FbxSurfaceMaterial::sAmbient
			, fbxsdk::FbxSurfaceMaterial::sAmbientFactor);

		// Spec
		tMtrlInfo.SpecularColor = get_material_data(_pMtrlSur
			, fbxsdk::FbxSurfaceMaterial::sSpecular
			, fbxsdk::FbxSurfaceMaterial::sSpecularFactor);

		// Emisv
		tMtrlInfo.EmissiveColor = get_material_data(_pMtrlSur
			, fbxsdk::FbxSurfaceMaterial::sEmissive
			, fbxsdk::FbxSurfaceMaterial::sEmissiveFactor);

		// Texture Name
		tMtrlInfo.strDiffuseTex = get_material_texture_name(_pMtrlSur, fbxsdk::FbxSurfaceMaterial::sDiffuse);
		tMtrlInfo.strNormalTex = get_material_texture_name(_pMtrlSur, fbxsdk::FbxSurfaceMaterial::sNormalMap);
		tMtrlInfo.strSpecularTex = get_material_texture_name(_pMtrlSur, fbxsdk::FbxSurfaceMaterial::sSpecular);
		tMtrlInfo.strEmissiveTex = get_material_texture_name(_pMtrlSur, fbxsdk::FbxSurfaceMaterial::sEmissive);

		return tMtrlInfo;
	}

	float3 FBXLoader::get_tangent(fbxsdk::FbxMesh* _pMesh
		, int _vertexIndex		 /*해당 정점의 인덱스*/
		, int _vertexOrder /*폴리곤 단위로 접근하는 순서*/)
	{
		float3 ret = float3::Zero;

		// 메쉬로부터 ElementTangent 정보를 얻어온다.
		fbxsdk::FbxGeometryElementTangent* Tangent = _pMesh->GetElementTangent();

		if (nullptr == Tangent) {
			return ret;
		}

		int	tangentIndex = _vertexOrder;

		// MappingMode와 ReferenceMode에 따라서 인덱스로 사용할 정보가 달라진다.
		if (Tangent->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
		{
			switch (Tangent->GetReferenceMode())
			{
			case fbxsdk::FbxGeometryElement::eIndexToDirect:
				tangentIndex = Tangent->GetIndexArray().GetAt(_vertexOrder);
				break;
			}
		}

		else if (Tangent->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
		{
			switch (Tangent->GetReferenceMode())
			{
			case fbxsdk::FbxGeometryElement::eDirect:
				tangentIndex = _vertexIndex;
				break;
			case fbxsdk::FbxGeometryElement::eIndexToDirect:
				tangentIndex = Tangent->GetIndexArray().GetAt(_vertexIndex);
				break;
			}
		}

		fbxsdk::FbxVector4	vTangent = Tangent->GetDirectArray().GetAt(tangentIndex);

		ret.x = (float)vTangent.mData[0];
		ret.y = (float)vTangent.mData[2];
		ret.z = (float)vTangent.mData[1];

		return ret;
	}

	float3 FBXLoader::get_binormal(fbxsdk::FbxMesh* _pMesh, int _vertexIndex, int _vertexOrder)
	{
		float3 ret = float3::Zero;
		// 메쉬로부터 ElementBinormal 정보를 얻어온다.
		fbxsdk::FbxGeometryElementBinormal* Binormal = _pMesh->GetElementBinormal();

		if (nullptr == Binormal) {
			return ret;
		}

		int	BinormalIndex = _vertexOrder;

		// MappingMode와 ReferenceMode에 따라서 인덱스로 사용할 정보가 달라진다.
		if (Binormal->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
		{
			switch (Binormal->GetReferenceMode())
			{
			case fbxsdk::FbxGeometryElement::eIndexToDirect:
				BinormalIndex = Binormal->GetIndexArray().GetAt(_vertexOrder);
				break;
			}
		}

		else if (Binormal->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
		{
			switch (Binormal->GetReferenceMode())
			{
			case fbxsdk::FbxGeometryElement::eDirect:
				BinormalIndex = _vertexIndex;
				break;
			case fbxsdk::FbxGeometryElement::eIndexToDirect:
				BinormalIndex = Binormal->GetIndexArray().GetAt(_vertexIndex);
				break;
			}
		}

		fbxsdk::FbxVector4	vBinormal = Binormal->GetDirectArray().GetAt(BinormalIndex);

		ret.x = (float)vBinormal.mData[0];
		ret.y = (float)vBinormal.mData[2];
		ret.z = (float)vBinormal.mData[1];

		return ret;
	}

	float3 FBXLoader::get_normal(fbxsdk::FbxMesh* _pMesh, int _vertexIndex, int _vertexOrder)
	{
		float3 ret = float3::Zero;

		// 메쉬로부터 ElementNormal 정보를 얻어온다.
		fbxsdk::FbxGeometryElementNormal* Normal = _pMesh->GetElementNormal();

		int	NormalIndex = _vertexOrder;

		// MappingMode와 ReferenceMode에 따라서 인덱스로 사용할 정보가 달라진다.
		if (Normal->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
		{
			switch (Normal->GetReferenceMode())
			{
			case fbxsdk::FbxGeometryElement::eIndexToDirect:
				NormalIndex = Normal->GetIndexArray().GetAt(_vertexOrder);
				break;
			}
		}

		else if (Normal->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
		{
			switch (Normal->GetReferenceMode())
			{
			case fbxsdk::FbxGeometryElement::eDirect:
				NormalIndex = _vertexIndex;
				break;
			case fbxsdk::FbxGeometryElement::eIndexToDirect:
				NormalIndex = Normal->GetIndexArray().GetAt(_vertexIndex);
				break;
			}
		}

		fbxsdk::FbxVector4	vNormal = Normal->GetDirectArray().GetAt(NormalIndex);

		
		ret.x = (float)vNormal.mData[0];
		ret.y = (float)vNormal.mData[2];
		ret.z = (float)vNormal.mData[1];

		return ret;
	}

	float2 FBXLoader::get_UV(fbxsdk::FbxMesh* _pMesh, int _vertexIndex, int _iUVIndex)
	{
		float2 ret = float2::Zero;

		//참고: 여러개의 UV좌표들이 존재할 수도 있다. 일단은 1개만 취급
		fbxsdk::FbxGeometryElementUV* pUV = _pMesh->GetElementUV();
		if (nullptr == pUV) {
			DEBUG_MESSAGE("UV좌표가 없습니다.\n");
			return ret;
		}

		UINT iUVIdx = _iUVIndex;

		//Control Point == Vertex Index
		if (pUV->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint) {
			switch (pUV->GetReferenceMode())
			{
			case fbxsdk::FbxGeometryElement::eDirect: {
				iUVIdx = _vertexIndex;
				break;
			}
			case fbxsdk::FbxGeometryElement::eIndexToDirect: {
				iUVIdx = pUV->GetIndexArray().GetAt(_vertexIndex);
				break;
			}
			default:
				ASSERT(false, "예상치 못한 FbxGeometryElement 타입입니다.");
				break;
			}
		}

		fbxsdk::FbxVector2 vUV = pUV->GetDirectArray().GetAt(iUVIdx);
		ret.x = (float)vUV.mData[0];
		ret.y = 1.f - (float)vUV.mData[1]; // fbx uv 좌표계는 좌하단이 0,0

		return ret;
	}

	float4 FBXLoader::get_material_data(fbxsdk::FbxSurfaceMaterial* _pSurface
		, const char* _pMtrlName
		, const char* _pMtrlFactorName)
	{
		fbxsdk::FbxDouble3  vMtrl{};
		fbxsdk::FbxDouble	dFactor = 0.;

		fbxsdk::FbxProperty tMtrlProperty = _pSurface->FindProperty(_pMtrlName);
		fbxsdk::FbxProperty tMtrlFactorProperty = _pSurface->FindProperty(_pMtrlFactorName);

		if (tMtrlProperty.IsValid() && tMtrlFactorProperty.IsValid())
		{
			vMtrl = tMtrlProperty.Get<fbxsdk::FbxDouble3>();
			dFactor = tMtrlFactorProperty.Get<fbxsdk::FbxDouble>();
		}

		float4 vRetVal = float4((float)vMtrl.mData[0] * (float)dFactor, (float)vMtrl.mData[1] * (float)dFactor, (float)vMtrl.mData[2] * (float)dFactor, (float)dFactor);
		return vRetVal;
	}

	std::string FBXLoader::get_material_texture_name(fbxsdk::FbxSurfaceMaterial* _pSurface, const char* _pMtrlProperty)
	{
		std::string retStr;

		fbxsdk::FbxProperty TextureProperty = _pSurface->FindProperty(_pMtrlProperty);
		if (TextureProperty.IsValid())
		{
			UINT iCnt = TextureProperty.GetSrcObjectCount();

			if (1 <= iCnt)
			{
				fbxsdk::FbxFileTexture* pFbxTex = TextureProperty.GetSrcObject<fbxsdk::FbxFileTexture>(0);
				if (nullptr != pFbxTex)
				{
					std::fs::path fullPath = pFbxTex->GetFileName();

					//절대 주소가 있을 경우 상대 주소를 만들어서 저장
					if (std::fs::exists(fullPath))
					{
						retStr = fullPath.string();
						//static std::fs::path resPath = PathManager::GetContentPathAbsolute(eResourceType::Model3D);

						//fullPath = fullPath.lexically_relative(resPath);

						//retStr = fullPath.string();
					}
				}
			}
		}

		return retStr;
	}

	void FBXLoader::load_textures()
	{
		//일단 텍스처를 Texture 컨텐츠 폴더로 옮겨준다.
		const std::fs::path& fbxPath = ResourceManager<Model3D>::get_inst().GetBaseDir();
		const std::fs::path& texPath = ResourceManager<Texture>::get_inst().GetBaseDir();

		//텍스처 폴더 발견 시 최초 한번 Texture 폴더로 이동시키기 위한 코드
		std::fs::path srcPathToDelete{};
		auto CopyAndLoadTex = 
			[&](std::string& _TextureRelativePath)->void
			{
				//비어있을경우 return
				if (_TextureRelativePath.empty()) {
					return;
				}

				//이동 원본 경로와 목표 경로를 만들어준다.
				std::fs::path srcPath = fbxPath / _TextureRelativePath;
				std::fs::path destPath = texPath / _TextureRelativePath;

				//src에 파일이 있는데 dest에 없을 경우 복사
				if (std::fs::exists(srcPath) && false == std::fs::exists(destPath))
				{
					std::fs::path destDir = destPath;
					destDir.remove_filename();
					if (false == std::fs::exists(destDir))
					{
						std::fs::create_directories(destDir);
					}

					//파일을 복사하고 
					std::fs::copy(srcPath, destPath);
				}

				//바로 Texture Load. 로드 실패 시 false 반환
				if (nullptr == ResourceManager<Texture>::get_inst().load(_TextureRelativePath))
				{
					_TextureRelativePath.clear();
					return;
				}
				else if (srcPathToDelete.empty())
				{
					srcPathToDelete = srcPath;
					srcPathToDelete.remove_filename();
				}
			};

		//순회를 돌며 텍스처 확인 후 이동시키고 로드까지 완료시킨다.
		for (UINT i = 0; i < mContainers.size(); ++i)
		{
			for (UINT j = 0; j < mContainers[i].vecMtrl.size(); ++j)
			{
				CopyAndLoadTex(mContainers[i].vecMtrl[j].strDiffuseTex);
				CopyAndLoadTex(mContainers[i].vecMtrl[j].strNormalTex);
				CopyAndLoadTex(mContainers[i].vecMtrl[j].strSpecularTex);
				CopyAndLoadTex(mContainers[i].vecMtrl[j].strEmissiveTex);
			}
		}
		
		//순회 다돌았는데 원본 폴더 경로 있으면 제거
		if (std::fs::exists(srcPathToDelete))
		{
			std::fs::remove_all(srcPathToDelete);
		}
	}

	//void FBXLoader::CreateMaterial(const std::filesystem::path& _strPath)
	//{
	//	//std::string strMtrlKey;
	//	//std::string strPath;

	//	for (UINT i = 0; i < mContainers.size(); ++i)
	//	{
	//		for (UINT j = 0; j < mContainers[i].vecMtrl.size(); ++j)
	//		{
	//			//std::fs::path strMtrlKey = mContainers[i].vecMtrl[j].strMtrlName;
	//			//if (strMtrlKey.empty())
	//			//{
	//			//	//파일 이름에서 확장자 제거하고 이름만 받아옴
	//			//	strMtrlKey = _strPath.stem();

	//			//	//번호 붙여줌
	//			//	strMtrlKey += "_Mtrl";
	//			//	strMtrlKey += std::to_string(j);
	//			//}
	//			//
	//			////.json 확장자 붙여 줌
	//			//strMtrlKey.replace_extension(name::path::extension::Material);

	//			////strPath = "material\\";
	//			////strPath += strMtrlKey + ".mtrl";

	//			//// 재질 이름
	//			//mContainers[i].vecMtrl[j].strMtrlName = strMtrlKey.string();

	//			//std::string strName = strPath;

	//			// 이미 로딩된 재질이면 로딩된 것을 사용
	//			std::shared_ptr<Material> pMaterial = ResourceManager<Material>::get_inst().find(mContainers[i].vecMtrl[j].strMtrlName);
	//			if (nullptr != pMaterial) {
	//				continue;
	//			}

	//			pMaterial = std::make_shared<Material>();

	//			// 상대경로가 곧 키
	//			pMaterial->set_resource_name(mContainers[i].vecMtrl[j].strMtrlName);

	//			//일단 기본 설정은 Deffered Shader 적용하는 걸로. 나중에 바꿀 것
	//			pMaterial->set_rendering_mode(eRenderingMode::deffered_opaque);

	//			const std::shared_ptr<GraphicsShader>& defferedShader = ResourceManager<GraphicsShader>::get_inst().find(name::defaultRes::shader::graphics::Deffered3DShader);

	//			ASSERT(nullptr == defferedShader, "Deffered Shader를 찾지 못했습니다.");

	//			pMaterial->set_shader(defferedShader);
	//			
	//			{
	//				std::shared_ptr<Texture> pTex = ResourceManager<Texture>::get_inst().load(mContainers[i].vecMtrl[j].strDiffuseTex);
	//				if (nullptr != pTex)
	//				{
	//					pMaterial->set_texture(eTextureSlot::Diffuse, pTex);
	//				}
	//			}
	//				
	//			{
	//				std::shared_ptr<Texture> pTex = ResourceManager<Texture>::get_inst().load(mContainers[i].vecMtrl[j].strNormalTex);
	//				if (nullptr != pTex)
	//				{
	//					pMaterial->set_texture(eTextureSlot::Normal, pTex);
	//				}
	//			}

	//			{
	//				std::shared_ptr<Texture> pTex = ResourceManager<Texture>::get_inst().load(mContainers[i].vecMtrl[j].strSpecularTex);
	//				if (nullptr != pTex)
	//				{
	//					pMaterial->set_texture(eTextureSlot::Specular, pTex);
	//				}
	//			}

	//			{
	//				std::shared_ptr<Texture> pTex = ResourceManager<Texture>::get_inst().find(mContainers[i].vecMtrl[j].strEmissiveTex);
	//				if (nullptr != pTex)
	//				{
	//					pMaterial->set_texture(eTextureSlot::Emissive, pTex);
	//				}
	//			}

	//			pMaterial->SetDiffuseColor(mContainers[i].vecMtrl[j].DiffuseColor);
	//			pMaterial->SetSpecularColor(mContainers[i].vecMtrl[j].SpecularColor);
	//			pMaterial->SetAmbientColor(mContainers[i].vecMtrl[j].AmbientColor);
	//			pMaterial->SetEmissiveColor(mContainers[i].vecMtrl[j].EmissiveColor);

	//			eResult result = ResourceManager<Material>::get_inst().save(pMaterial);

	//			if (eResult_fail(result))
	//			{
	//				ERROR_MESSAGE("FBX 변환 에러: Material 저장 실패");
	//			}

	//			ResourceManager<Material>::get_inst().insert(pMaterial->get_resource_name(), pMaterial);
	//		}
	//	}
	//}

	void FBXLoader::load_skeleton(fbxsdk::FbxNode* _pNode)
	{
		//mBones.reserve(200);

		//Root 노드는 Skeletal Node가 될 수 없다
		//https://download.autodesk.com/us/fbx/20102/fbx_sdk_help/index.html?url=WS8e4c2438b09b7f9c-5fe157071197ccd9b09-7ffe.htm,topicNumber=d0e4048
		int childCount = _pNode->GetChildCount();
		for (int i = 0; i < childCount; ++i)
		{
			load_bone_recursive(_pNode->GetChild(i), 0, 0, -1);
		}
	}

	void FBXLoader::load_bone_recursive(fbxsdk::FbxNode* _pNode, int _iDepth, int _iIdx, int _iParentIdx)
	{
		fbxsdk::FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

		if (pAttr && pAttr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
		{
			tFBXBone bone{};

			bone.strBoneName = _pNode->GetName();
			if (mbMixamo)
			{
				bone.strBoneName.erase(0, 10);
			}

			bone.Depth = _iDepth++;
			bone.ParentIndx = _iParentIdx;

			mBones.push_back(bone);

			//Bone Matrix는 애니메이션에서 로드함
		}

		int iChildCount = _pNode->GetChildCount();
		for (int i = 0; i < iChildCount; ++i)
		{
			load_bone_recursive(_pNode->GetChild(i), _iDepth, (int)mBones.size(), _iIdx);
		}
	}

	void FBXLoader::load_animation_clip()
	{
		int iAnimCount = mAnimNames.GetCount();
		mAnimClips.reserve(iAnimCount);

		fbxsdk::FbxTime::EMode timeMode = mScene->GetGlobalSettings().GetTimeMode();

		for (int i = 0; i < iAnimCount; ++i)
		{
			fbxsdk::FbxAnimStack* pAnimStack = mScene->FindMember<fbxsdk::FbxAnimStack>(mAnimNames[i]->Buffer());

			if (nullptr == pAnimStack) {
				continue;
			}

			//FbxAnimEvaluator* pevaluator = mScene->GetAnimationEvaluator();
			//mScene->SetCurrentAnimationStack();
			tFBXAnimClip clip{};

			clip.strName = pAnimStack->GetName();
			if (clip.strName.empty()) {
				clip.strName = mFileName + "_Anim_" + std::to_string(i);
			}
			
			//믹사모인지 확인
			if (strMixamo == clip.strName)
			{
				mbMixamo = true;
			}

			fbxsdk::FbxTakeInfo* take = mScene->GetTakeInfo(clip.strName.c_str());
			clip.StartTime = take->mLocalTimeSpan.GetStart();
			clip.EndTime = take->mLocalTimeSpan.GetStop();
			
			// GetFrameCount 함수를 호출하고  time모드를 넣어주면 시간을 프레임으로
			// 변환해준다. 몇프레임 짜리 애니메이션 인지를 구해준다.
			clip.TimeLength = clip.EndTime.GetFrameCount(timeMode) - clip.StartTime.GetFrameCount(timeMode);
			clip.TimeMode = timeMode;

			mAnimClips.push_back(clip);
		}
	}

	void FBXLoader::triangulate(fbxsdk::FbxNode* _pNode)
	{
		fbxsdk::FbxNodeAttribute* pAttr = _pNode->GetNodeAttribute();

		if (pAttr &&
			(pAttr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh
				|| pAttr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbs
				|| pAttr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbsSurface))
		{
			fbxsdk::FbxGeometryConverter converter(mManager);
			converter.Triangulate(pAttr, true);
		}

		int iChildCount = _pNode->GetChildCount();

		for (int i = 0; i < iChildCount; ++i)
		{
			triangulate(_pNode->GetChild(i));
		}
	}

	void FBXLoader::load_animation_data(tFBXContainer& _container, fbxsdk::FbxMesh* _pMesh)
	{
		int iSkinCount = _pMesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
		if (iSkinCount <= 0) {
			return;
		}

		// 메쉬의 정점 수를 얻어온다.
		int	vertexCount = _pMesh->GetControlPointsCount();

		// 정점의 가중치 정보와 본인덱스 정보는 정점 수만큼
		// 만들어져야 한다.
		_container.bAnimation = true;
		_container.vecBlendWeight.resize(vertexCount);
		_container.vecBlendIndex.resize(vertexCount);

		fbxsdk::FbxAMatrix matTransform = GetTransform(_pMesh->GetNode());

		//Skin 갯수만큼 반복하며 읽어준다.
		for (int i = 0; i < iSkinCount; ++i)
		{
			fbxsdk::FbxSkin* skin = static_cast<fbxsdk::FbxSkin*>(_pMesh->GetDeformer(i, fbxsdk::FbxDeformer::eSkin));
			if (nullptr == skin)
				continue;

			fbxsdk::FbxSkin::EType skinningType = skin->GetSkinningType();

			if (skinningType == fbxsdk::FbxSkin::eRigid ||
				skinningType == fbxsdk::FbxSkin::eLinear ||
				skinningType == fbxsdk::FbxSkin::eBlend)
			{
				//Cluster: 관절을 의미
				int clusterCount = skin->GetClusterCount();
				for (int j = 0; j < clusterCount; ++j)
				{
					fbxsdk::FbxCluster* cluster = skin->GetCluster(j);
					if (nullptr == cluster->GetLink())
						continue;

					std::string boneName = cluster->GetLink()->GetName();
					if (mbMixamo)
					{
						boneName.erase(0, 10);
					}
					int boneIndex = FindBoneIndex(boneName);

					load_index_weights(_container, cluster, boneIndex);

					load_offset_matrices(_container, cluster, matTransform, boneIndex);

					//Bone의 로컬스페이스 위치
					mBones[boneIndex].matBone = matTransform;

					load_keyframe_transform(_container, _pMesh->GetNode(), cluster, matTransform, boneIndex);
				}
			}
		}

		CheckWeightAndIndices(_container, _pMesh);
	}


	void FBXLoader::CheckWeightAndIndices(tFBXContainer& _container, fbxsdk::FbxMesh* _pMesh)
	{
		std::unordered_map<int, std::vector<tFBXWeight>>::iterator iter = _container.mapWeights.begin();
		std::unordered_map<int, std::vector<tFBXWeight>>::iterator iterEnd = _container.mapWeights.end();

		for (iter = _container.mapWeights.begin(); iter != iterEnd; ++iter)
		{
			if (iter->second.size() > 4)
			{
				// 가중치 값에 따라 내림차순 정렬한다.
				sort(iter->second.begin(), iter->second.end(), 
					[](const tFBXWeight& lhs, const tFBXWeight& rhs)
					{
						return lhs.Weight > rhs.Weight;
					}
				);

				//1이 넘어갈경우 처음 부분에 더해줌
				double	Sum = 0.0;
				for (int i = 0; i < 4; ++i)
				{
					Sum += iter->second[i].Weight;
				}
				double	Interpolate = 1.f - Sum;

				std::vector<tFBXWeight>::iterator	iterErase = iter->second.begin() + 4;

				iter->second.erase(iterErase, iter->second.end());
				iter->second[0].Weight += Interpolate;
			}

			uint4	vIndex{};
			float4	vWeight{};

			for (int i = 0; i < iter->second.size() && i < 4; ++i)
			{
				//vIndex를 uint* 형태로 만든뒤 배열 형태로 접근한다
				reinterpret_cast<uint*>(&vIndex)[i] = (uint)iter->second[i].BoneIdx;
				reinterpret_cast<float*>(&vWeight)[i] = (float)iter->second[i].Weight;
			}

			_container.vecBlendWeight[iter->first] = vWeight;
			_container.vecBlendIndex[iter->first] = vIndex;
		}
	}

	int FBXLoader::GetAllNodesCountRecursive(fbxsdk::FbxNode* _pNode)
	{
		int size = _pNode->GetChildCount();

		int retInt = size;
		for (int i = 0; i < size; ++i)
		{
			retInt += GetAllNodesCountRecursive(_pNode->GetChild(i));
		}

		return retInt;
	}



	void FBXLoader::load_keyframe_transform(tFBXContainer& _container, fbxsdk::FbxNode* _pNode, fbxsdk::FbxCluster* _pCluster
		, const fbxsdk::FbxAMatrix& _matNodeTransform, int _bondIndex)
	{
		// Reflect Matrix(축 orientation을 변경)
		fbxsdk::FbxVector4 V0 = { 1.0, 0.0, 0.0, 0.0 };
		fbxsdk::FbxVector4 V1 = { 0.0, 0.0, 1.0, 0.0 };
		fbxsdk::FbxVector4 V2 = { 0.0, 1.0, 0.0, 0.0 };
		fbxsdk::FbxVector4 V3 = { 0.0, 0.0, 0.0, 1.0 };
		fbxsdk::FbxAMatrix matReflect;
		matReflect.mData[0] = V0;
		matReflect.mData[1] = V1;
		matReflect.mData[2] = V2;
		matReflect.mData[3] = V3;

		if (mbMixamo)
		{
			std::vector<tFBXAnimClip>::iterator	iter;
			std::vector<tFBXAnimClip>::iterator	iterEnd = mAnimClips.end();

			for (iter = mAnimClips.begin(); iter != iterEnd;)
			{
				if ("mixamo.com" != (*iter).strName)
				{
					iter = mAnimClips.erase(iter);
					iterEnd = mAnimClips.end();
				}

				else
					++iter;
			}
		}

		for (size_t i = 0; i < mAnimClips.size(); ++i)
		{
			fbxsdk::FbxLongLong	Start = mAnimClips[i].StartTime.GetFrameCount(mAnimClips[i].TimeMode);
			fbxsdk::FbxLongLong	End = mAnimClips[i].EndTime.GetFrameCount(mAnimClips[i].TimeMode);

			mAnimClips[i].KeyFramesPerBone[_bondIndex].BoneIndex = _bondIndex;

			// 전체 프레임 수만큼 반복한다.
			for (fbxsdk::FbxLongLong j = Start; j <= End; ++j)
			{
				fbxsdk::FbxTime	Time = {};

				// 현재 프레임에 해당하는 fbxsdk::FbxTime을 만들어낸다.
				Time.SetFrame(j, mAnimClips[i].TimeMode);

				// EvaluateGlobalTransform
				//해당 시간의 위치 * 현재 노드 위치 = 본 스페이스
				fbxsdk::FbxAMatrix	matOffset = _pNode->EvaluateGlobalTransform(Time) * _matNodeTransform;

				//역행렬 곱해줘서 로컬 스페이스로 돌린 후 부모 변환행렬을 곱해줌(로컬 스페이스에서)
				fbxsdk::FbxAMatrix	matCur = matOffset.Inverse() * _pCluster->GetLink()->EvaluateGlobalTransform(Time);

				//축 orientation 변환
				matCur = matReflect * matCur * matReflect;

				tFBXKeyFrame KeyFrame = {};

				KeyFrame.Time = Time.GetSecondDouble();
				KeyFrame.matTransform = matCur;

				mAnimClips[i].KeyFramesPerBone[_bondIndex].vecKeyFrame.push_back(KeyFrame);
			}
		}
	}

	void FBXLoader::load_offset_matrices(tFBXContainer& _container, fbxsdk::FbxCluster* _pCluster
		, const fbxsdk::FbxAMatrix& _matNodeTransform
		, int _bondIndex)
	{
		fbxsdk::FbxAMatrix matClusterTrans;
		fbxsdk::FbxAMatrix matClusterLinkTrans;

		//자신의 이동 행렬
		_pCluster->GetTransformMatrix(matClusterTrans);

		//부모까지의 이동 행렬
		_pCluster->GetTransformLinkMatrix(matClusterLinkTrans);

		// Reflect Matrix(축 orientation을 변경)
		fbxsdk::FbxVector4 V0 = { 1.0, 0.0, 0.0, 0.0 };
		fbxsdk::FbxVector4 V1 = { 0.0, 0.0, 1.0, 0.0 };
		fbxsdk::FbxVector4 V2 = { 0.0, 1.0, 0.0, 0.0 };
		fbxsdk::FbxVector4 V3 = { 0.0, 0.0, 0.0, 1.0 };
		fbxsdk::FbxAMatrix matReflect;
		matReflect.mData[0] = V0;
		matReflect.mData[1] = V1;
		matReflect.mData[2] = V2;
		matReflect.mData[3] = V3;

		/*
		1 0 0 0   1 2 3 4   1 0 0 0
		0 0 1 0   5 6 7 8   0 0 1 0
		0 1 0 0 * 9 0 1 2 * 0 1 0 0
		0 0 0 1   3 4 5 6   0 0 0 1

		1 2 3 4   1 0 0 0
		9 0 1 2   0 0 1 0
		5 6 7 8 * 0 1 0 0
		3 4 5 6   0 0 0 1

		1 3 2 4
		9 1 0 2
		5 7 6 8
		3 5 4 6
		*/

		fbxsdk::FbxAMatrix matOffset;

		//부모 행렬의 역행렬 * 자신의 변환행렬 => 원점(로컬 스페이스)로 돌아가서 자신의 변환행렬만큼만 이동
		// * _matNodeTransform => 해당 메쉬의 로컬위치를 곱해줌
		//결론: 특정 본의 로컬스페이스 좌표
		matOffset = matClusterLinkTrans.Inverse() * matClusterTrans * _matNodeTransform;

		//Axis orientation 바꿔주기
		matOffset = matReflect * matOffset * matReflect;

		mBones[_bondIndex].matOffset = matOffset;
	}


	void FBXLoader::load_index_weights(tFBXContainer& _container, fbxsdk::FbxCluster* _pCluster, int _bondIndex)
	{
		int iIndicesCount = _pCluster->GetControlPointIndicesCount();
		for (int i = 0; i < iIndicesCount; ++i)
		{
			tFBXWeight weight = {};

			// 각 정점에게 본 인덱스 정보와, 가중치 값을 알린다.
			weight.BoneIdx = _bondIndex;
			weight.Weight = _pCluster->GetControlPointWeights()[i];

			int vertexIndex = _pCluster->GetControlPointIndices()[i];

			_container.mapWeights[vertexIndex].push_back(weight);
		}
	}



	int FBXLoader::FindBoneIndex(const std::string& _strBoneName)
	{
		int retIdx = -1;

		for (UINT i = 0; i < mBones.size(); ++i)
		{
			if (mBones[i].strBoneName == _strBoneName)
			{
				retIdx = (int)i;
				break;
			}
		}

		return retIdx;
	}

	fbxsdk::FbxAMatrix FBXLoader::GetTransform(fbxsdk::FbxNode* _pNode)
	{
		const fbxsdk::FbxVector4 vT = _pNode->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
		const fbxsdk::FbxVector4 vR = _pNode->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
		const fbxsdk::FbxVector4 vS = _pNode->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);

		return fbxsdk::FbxAMatrix(vT, vR, vS);
	}

}
