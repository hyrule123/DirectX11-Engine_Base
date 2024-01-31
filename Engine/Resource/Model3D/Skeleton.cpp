
#include "Skeleton.h"



#include "../../Util/define_Util.h"

#include "../../DefaultShader/Resource.hlsli"

#include "../../GPU/StructBuffer.h"

#include "../../Manager/ResourceManager.h"


#include "Model3D.h"


#include "FBXLoader.h"


#include <cctype>

namespace ehw
{
	
	Skeleton::Skeleton()
		: iResource(typeid(Skeleton))
		, m_vecBones{}
		, m_pBoneOffset{}
		, m_animations{}
	{
	}

	Skeleton::~Skeleton()
	{
	}

	eResult Skeleton::Save(const std::fs::path& _basePath, const std::fs::path& _strKeyPath)
	{
		//상위 디렉토리 있는지 테스트
		{
			std::fs::path checkDir = _strKeyPath;
			checkDir.remove_filename();
			if (false == std::fs::is_directory(checkDir))
			{
				ERROR_MESSAGE("Skeleton은 반드시 폴더 안에 저장되어야 합니다.");
				return eResult::Fail;
			}
		}

		std::fs::path fullPath = _basePath / _strKeyPath;
		//Skeleton
		eResult result = SaveFile(fullPath);
		if (eResultFail(result))
		{
			ERROR_MESSAGE("스켈레톤 정보 로드 실패.");
			return eResult::Fail;
		}
		
		//Animation3D는 Skeleton에 종속되므로 ResourceManager를 사용하지 않음 -> SaveFile을 통해 저장한다.
		{
			fullPath.remove_filename();

			for (const auto& iter : m_animations)
			{
				result = iter.second->SaveFile(fullPath / iter.first);
				if (eResultFail(result))
				{
					std::wstringstream errmsg{};
					errmsg << L"애니메이션 저장 실패\b실패한 애니메이션 이름: ";
					errmsg << StringConverter::UTF8_to_Unicode(iter.first);
					ERROR_MESSAGE_W(errmsg.str().c_str());

					continue;
				}
			}
		}


		return SaveFile(_basePath / _strKeyPath);
	}
	eResult Skeleton::Load(const std::fs::path& _basePath, const std::fs::path& _strKeyPath)
	{
		std::fs::path fullPath = _basePath / _strKeyPath;

		//Skeleton
		eResult result = LoadFile(fullPath);
		if (eResultFail(result))
		{
			ERROR_MESSAGE("스켈레톤 정보 로드 실패.");
			return eResult::Fail;
		}

		//Animation 3D: Skeleton과 동일한 경로의 .a3d 파일들
		try
		{
			//파일명 제거
			fullPath.remove_filename();

			if (false == std::fs::is_directory(fullPath))
			{
				ERROR_MESSAGE("경로가 잘못되었습니다.");
				return eResult::Fail_Open;
			}

			//내부 순회돌아주면서 a3d 확장자 파일을 모두 불러온다.
			for (const auto& dirIter : std::fs::directory_iterator(fullPath))
			{
				const std::fs::path& filePath = dirIter.path();

				if (std::fs::is_directory(filePath))
				{
					continue;
				}
				else if (strKey::path::extension::Anim3D != filePath.extension())
				{
					continue;
				}
				
				std::shared_ptr<Animation3D> a3d = std::make_shared<Animation3D>();
				eResult result = a3d->LoadFile(filePath);
				if (eResultFail(result))
				{
					std::wstringstream errmsg;
					errmsg << L"3D 애니메이션 로드 실패\n경로: ";
					errmsg << filePath.wstring();
					errmsg << L"\n";

					ERROR_MESSAGE_W(errmsg.str().c_str());

					//애니메이션 하나 로드 못하는게 전체 문제는 아니므로 continue
					continue;
				}

				std::string animName = filePath.filename().replace_extension().string();
				m_animations.insert(std::make_pair(animName, a3d));
			}
		}
		catch (const std::runtime_error& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail;
		}
		

		return eResult::Success;
	}

	eResult Skeleton::Serialize(BinarySerializer& _ser)
	{
		_ser << m_vecBones.size();
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			//std::string			strBoneName{};
			//int					iDepth;
			//int					iParentIndx;
			//MATRIX				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
			//MATRIX				matBone;	// 이거 안씀
			_ser << m_vecBones[i].strBoneName;
			_ser << m_vecBones[i].iDepth;
			_ser << m_vecBones[i].iParentIndx;
			_ser << m_vecBones[i].matOffset;
			_ser << m_vecBones[i].matBone;
		}

		return eResult::Success;
	}



	eResult Skeleton::DeSerialize(BinarySerializer& _ser)
	{
		size_t size{};
		_ser >> size;
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			//std::string			strBoneName{};
			//int					iDepth;
			//int					iParentIndx;
			//MATRIX				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
			//MATRIX				matBone;	// 이거 안씀
			_ser >> m_vecBones[i].strBoneName;
			_ser >> m_vecBones[i].iDepth;
			_ser >> m_vecBones[i].iParentIndx;
			_ser >> m_vecBones[i].matOffset;
			_ser >> m_vecBones[i].matBone;
		}
		CreateBoneOffsetSBuffer();

		return eResult::Success;
	}

	eResult Skeleton::CreateFromFBX(FBXLoader* _fbxLoader)
	{
		const std::vector<tFBXBone>& vecBone = _fbxLoader->GetBones();
		if (vecBone.empty())
		{
			return eResult::Fail_Empty;
		}

		UINT iFrameCount = 0;
		m_vecBones.reserve(vecBone.size());
		for (UINT i = 0; i < vecBone.size(); ++i)
		{
			m_vecBones.push_back(tMTBone{});
			tMTBone& bone = m_vecBones.back();

			bone.iDepth = vecBone[i].Depth;
			bone.iParentIndx = vecBone[i].ParentIndx;
			bone.matBone = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i].matBone);
			bone.matOffset = FBXLoader::GetMatrixFromFbxMatrix(vecBone[i].matOffset);
			bone.strBoneName = vecBone[i].strBoneName;
		}
		CreateBoneOffsetSBuffer();

		const auto& animClip = _fbxLoader->GetAnimations();
		for (size_t i = 0; i < animClip.size(); ++i)
		{
			std::unique_ptr<Animation3D> anim = std::make_unique<Animation3D>();

			eResult result = anim->LoadFromFBX(shared_from_this_T<Skeleton>(), &animClip[i]);
			if (eResultFail(result))
			{
				ERROR_MESSAGE("애니메이션 생성 실패");
				return result;
			}
			
			std::string animName = anim->GetStrKey();
			if (animName.empty())
			{
				//애니메이션이 1000개를 넘을거같진 않으니 3자리까지만 고정
				size_t numAnim = animClip.size();
				int digits = 3;
				do
				{
					numAnim /= (size_t)10;
					--digits;
				} while (numAnim);

				if (digits < 0)
					digits = 0;

				animName = "Anim_";
				for (int i = 0; i < digits; ++i)
				{
					animName += "0";
				}
				
				animName += std::to_string(i);
			}

			m_animations.insert(std::make_pair(animName, anim.release()));
		}
		return eResult::Success;
	}

	bool Skeleton::CopyAnimationFromOther(const Skeleton& _other, const std::fs::path& _saveDir)
	{
		//순회를 돌아주면서 내 스켈레톤 인덱스와 매칭되는 상대 스켈레톤 인덱스를 계산
		if (m_vecBones.size() != _other.m_vecBones.size())
			return false;

		std::vector<int> matchingIndices(m_vecBones.size());
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			//내 본과 일치하는 본의 인덱스를 찾는다.
			int otherIdx = _other.FindSameBoneIndex(m_vecBones[i]);

			//일치하는 본이 없으면 Return
			if (0 > otherIdx)
				return false;

			//내 본의 i번쨰 = 상대 본의 otherIdx 번쨰
			matchingIndices[i] = otherIdx;
		}

		for (const auto& otherAnim : _other.m_animations)
		{
			//일단 다른쪽의 애니메이션을 복사해온다.
			std::shared_ptr<Animation3D> ourAnim = std::shared_ptr<Animation3D>(otherAnim.second->Clone());

			//스켈레톤 주소를 자신의것으로 변경
			ourAnim->SetSkeleton(shared_from_this_T<Skeleton>());

			//위에서 찾은 일치하는 Bone 번호 인덱스를 가져온다.
			for (size_t i = 0; i < matchingIndices.size(); ++i)
			{
				//인덱스가 서로 다른 경우에 인덱스 번호를 바꿔준다
				if ((int)i != matchingIndices[i])
				{
					std::swap(ourAnim->m_KeyFramesPerBone[i], ourAnim->m_KeyFramesPerBone[matchingIndices[i]]);
				}
			}

			std::string strKey = otherAnim.first;
			auto iter = m_animations.find(strKey);
			while (iter != m_animations.end())
			{
				strKey += "+";
				iter = m_animations.find(strKey);
			}

			std::fs::path filePath = _saveDir.filename();
			filePath /= strKey;
			
			if (eResultFail(ourAnim->SaveFile(filePath)))
				return false;

			//우리 애니메이션 쪽에 등록
			m_animations.insert(std::make_pair(strKey, ourAnim));
		}

		return true;
	}

	int Skeleton::FindSameBoneIndex(const tMTBone& _other) const
	{
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			if (
				m_vecBones[i].strBoneName == _other.strBoneName
				&&
				m_vecBones[i].iDepth == _other.iDepth
				&&
				m_vecBones[i].iParentIndx == _other.iParentIndx
				)
			{
				return (int)i;
			}
		}

		return -1;
	}

	void Skeleton::CreateBoneOffsetSBuffer()
	{
		// BoneOffet 행렬
		std::vector<MATRIX> vecOffset;
		vecOffset.reserve(m_vecBones.size());
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i].matOffset);
		}

		//Create
		
		StructBuffer::Desc desc{};
		desc.eSBufferType = eStructBufferType::READ_ONLY;
		desc.REGISLOT_t_SRV = Register_t_g_BoneOffsetArray;
		m_pBoneOffset = std::make_unique<StructBuffer>(desc);
		m_pBoneOffset->Create<MATRIX>(vecOffset.size(), vecOffset.data(), vecOffset.size());
	}

	std::shared_ptr<Animation3D> Skeleton::FindAnimation(const std::string_view _strAnimName)
	{
		std::shared_ptr<Animation3D> retPtr = nullptr;
		const auto& iter = m_animations.find(_strAnimName);
		if (iter != m_animations.end())
		{
			retPtr = iter->second;
		}
		return retPtr;
	}

}

