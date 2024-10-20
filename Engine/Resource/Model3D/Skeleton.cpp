#include "Engine/Resource/Model3D/Skeleton.h"


#include "Engine/Util/define_Util.h"
#include "Engine/Util/StringConverter.h"
#include "Engine/Util/Serialize/BinarySerializer.h"

#include "Engine/DefaultShader/Common_register.hlsli"

#include "Engine/GPU/StructBuffer.h"
#include "Engine/GPU/ConstBuffer.h"

#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Model3D/Model3D.h"
#include "Engine/Resource/Model3D/Skeleton.h"
#include "Engine/Resource/Model3D/FBXLoader.h"
#include "Engine/Resource/Model3D/Animation3D.h"
#include "Engine/Resource/Shader/ComputeShaders/Animation3D_ComputeShader.h"

#include "Engine/Game/Component/Animator/Animation3D_PlayData.h"

#include <cctype>

namespace ehw
{
	Skeleton::Skeleton()
		: Resource(Skeleton::concrete_class_name)
		, m_vecBones{}
		, m_pBoneOffset{}
		, m_animations{}
		, m_final_matrix_buffer{}
	{
		StructBuffer::Desc desc{};
		desc.GPU_register_t_SRV = GPU::Register::t::g_FinalBoneMatrixArray;
		desc.TargetStageSRV = eShaderStageFlag::Vertex;
		desc.GPU_register_u_UAV = GPU::Register::u::g_FinalBoneMatrixArrayRW;
		desc.eSBufferType = eStructBufferType::READ_WRITE;
		m_final_matrix_buffer = std::make_unique<StructBuffer>();
		m_final_matrix_buffer->init<MATRIX>(desc);
	}

	Skeleton::~Skeleton()
	{
	}

	eResult Skeleton::save(const std::fs::path& _basePath, const std::fs::path& _resource_name) const
	{
		//디렉토리가 존재하는지 확인한다.
		{
			if (false == std::fs::is_directory((_basePath / _resource_name).remove_filename()))
			{
				ERROR_MESSAGE("Skeleton은 반드시 폴더 안에 저장되어야 합니다.");
				return eResult::Fail;
			}
		}

		std::fs::path fullPath = _basePath / _resource_name;
		fullPath.replace_extension(name::path::extension::Skeleton);
		//Skeleton
		eResult result = SaveFile_Binary(fullPath);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("스켈레톤 정보 로드 실패.");
			return eResult::Fail;
		}
		
		//Animation3D는 Skeleton에 종속되므로 ResourceManager를 사용하지 않음 -> SaveFile_Binary을 통해 저장한다.
		{
			for (const auto& iter : m_animations)
			{
				fullPath.replace_filename(iter.first);
				fullPath.replace_extension(name::path::extension::Anim3D);
				result = iter.second->SaveFile_Binary(fullPath);
				if (eResult_fail(result))
				{
					std::wstringstream errmsg{};
					errmsg << L"애니메이션 저장 실패\b실패한 애니메이션 이름: ";
					errmsg << StringConverter::UTF8_to_Unicode(iter.first);
					ERROR_MESSAGE_W(errmsg.str().c_str());

					continue;
				}
			}
		}

		return result;
	}
	eResult Skeleton::load(const std::fs::path& _basePath, const std::fs::path& _resource_name)
	{
		std::fs::path fullPath = _basePath / _resource_name;

		fullPath.replace_extension(name::path::extension::Skeleton);
		//Skeleton
		eResult result = LoadFile_Binary(fullPath);
		if (eResult_fail(result))
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
				else if (name::path::extension::Anim3D != filePath.extension())
				{
					continue;
				}
				
				std::shared_ptr<Animation3D> a3d = std::make_shared<Animation3D>();
				eResult result = a3d->LoadFile_Binary(filePath);
				if (eResult_fail(result))
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

	eResult Skeleton::serialize_binary(BinarySerializer* _ser) const
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		BinarySerializer& ser = *_ser;

		ser << m_vecBones.size();
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			//std::string			strBoneName{};
			//int					iDepth;
			//int					iParentIndx;
			//MATRIX				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
			//MATRIX				matBone;	// 이거 안씀
			ser << m_vecBones[i].strBoneName;
			ser << m_vecBones[i].iDepth;
			ser << m_vecBones[i].iParentIndx;
			ser << m_vecBones[i].matOffset;
			ser << m_vecBones[i].matBone;
		}

		return eResult::Success;
	}



	eResult Skeleton::deserialize_binary(const BinarySerializer* _ser)
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		const BinarySerializer& ser = *_ser;

		size_t size{};
		ser >> size;
		m_vecBones.resize(size);
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			//std::string			strBoneName{};
			//int					iDepth;
			//int					iParentIndx;
			//MATRIX				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
			//MATRIX				matBone;	// 이거 안씀
			ser >> m_vecBones[i].strBoneName;
			ser >> m_vecBones[i].iDepth;
			ser >> m_vecBones[i].iParentIndx;
			ser >> m_vecBones[i].matOffset;
			ser >> m_vecBones[i].matBone;
		}
		create_bone_offset_buffer();

		return eResult::Success;
	}

	eResult Skeleton::create_from_FBX(FBXLoader* _fbxLoader)
	{
		const std::vector<tFBXBone>& vecBone = _fbxLoader->get_bones();
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
			bone.matBone = FBXLoader::get_matrix_from_FbxAMatrix(vecBone[i].matBone);
			bone.matOffset = FBXLoader::get_matrix_from_FbxAMatrix(vecBone[i].matOffset);
			bone.strBoneName = vecBone[i].strBoneName;
		}
		create_bone_offset_buffer();

		const auto& animClip = _fbxLoader->get_animations();
		for (size_t i = 0; i < animClip.size(); ++i)
		{
			std::unique_ptr<Animation3D> anim = std::make_unique<Animation3D>();

			std::shared_ptr<Skeleton> sklt =
				shared_from_this_cast<Skeleton>();

			eResult result = anim->load_from_fbx(sklt, &animClip[i]);

			if (eResult_fail(result))
			{
				ERROR_MESSAGE("애니메이션 생성 실패");
				return result;
			}
			
			
			std::string animName(anim->get_resource_name());
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

	bool Skeleton::copy_animation_from_other(const Skeleton& _other, const std::fs::path& _saveDir)
	{
		//순회를 돌아주면서 내 스켈레톤 인덱스와 매칭되는 상대 스켈레톤 인덱스를 계산
		if (m_vecBones.size() != _other.m_vecBones.size())
		{
			ERROR_MESSAGE("본 정보가 일치하지 않습니다.");
			return false;
		}
			

		std::vector<int> matchingIndices(m_vecBones.size());
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			//내 본과 일치하는 본의 인덱스를 찾는다.
			int otherIdx = _other.find_same_bone_index(m_vecBones[i]);

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
			std::shared_ptr<Skeleton> my_shared_ptr =
				std::static_pointer_cast<Skeleton>(shared_from_this_cast<Skeleton>());
			ourAnim->set_skeleton(my_shared_ptr);

			//위에서 찾은 일치하는 Bone 번호 인덱스를 가져온다.
			for (size_t i = 0; i < matchingIndices.size(); ++i)
			{
				//인덱스가 서로 다른 경우에 인덱스 번호를 바꿔준다
				if ((int)i != matchingIndices[i])
				{
					std::swap(ourAnim->m_KeyFramesPerBone[i], ourAnim->m_KeyFramesPerBone[matchingIndices[i]]);
				}
			}

			std::string name = otherAnim.first;
			auto iter = m_animations.find(name);
			while (iter != m_animations.end())
			{
				name += "+";
				iter = m_animations.find(name);
			}

			std::fs::path filePath = _saveDir.filename();
			filePath /= name;
			
			if (eResult_fail(ourAnim->SaveFile_Binary(filePath)))
				return false;

			//우리 애니메이션 쪽에 등록
			m_animations.insert(std::make_pair(name, ourAnim));
		}

		return true;
	}

	int Skeleton::find_same_bone_index(const tMTBone& _other) const
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

	void Skeleton::create_bone_offset_buffer()
	{
		// BoneOffet 행렬
		std::vector<MATRIX> vecOffset;
		vecOffset.reserve(m_vecBones.size());
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i].matOffset);
		}

		//CreateBuffer
		StructBuffer::Desc desc{};
		desc.eSBufferType = eStructBufferType::READ_ONLY;
		desc.GPU_register_t_SRV = GPU::Register::t::g_BoneOffsetArray;
		m_pBoneOffset = std::make_shared<StructBuffer>();

		eResult result = m_pBoneOffset->init<MATRIX>(desc, (UINT)vecOffset.size(), vecOffset.data(), (UINT)vecOffset.size());
		
		ASSERT_DEBUG(eResult_success(result), "본 오프셋 버퍼 생성 실패");
	}

	std::shared_ptr<Animation3D> Skeleton::find_animation(const std::string_view _strAnimName)
	{
		std::shared_ptr<Animation3D> retPtr = nullptr;
		const auto& iter = m_animations.find(_strAnimName);
		if (iter != m_animations.end())
		{
			retPtr = iter->second;
		}
		return retPtr;
	}


	void Skeleton::compute_bone_final_matrix()
	{
		if (m_compute_queue.empty()) {
			return;
		}

		//계산해야 하는 인스턴스 개수만큼 구조화 버퍼 크기를 늘려준다.
		uint size = (uint)(m_vecBones.size() * m_compute_queue.size());
		m_final_matrix_buffer->resize(size);

		//UAV에 바인딩한 뒤
		m_final_matrix_buffer->bind_buffer_as_UAV();

		//각자 애니메이션에 대해 업데이트를 수행
		for (uint i = 0; i < (uint)m_compute_queue.size(); ++i) {
			m_compute_queue[i]->update_final_matrix(i, m_final_matrix_buffer.get());
		}
		//계산 완료된 큐는 바로 비워준다.
		m_compute_queue.clear();

		//UAV 연결 해제
		m_final_matrix_buffer->unbind_buffer();
	}

	void Skeleton::bind_bone_final_matrix_SRV()
	{
		m_final_matrix_buffer->bind_buffer_as_SRV();
	}

}

