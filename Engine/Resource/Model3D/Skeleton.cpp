
#include "Skeleton.h"


#include <Engine/Util/define_Util.h>
#include <Engine/Util/StringConverter.h>
#include <Engine/Util/Serialize/BinarySerializer.h>

#include <Engine/DefaultShader/Common_register.hlsli>

#include <Engine/GPU/StructBuffer.h>
#include <Engine/GPU/ConstBuffer.h>

#include <Engine/Manager/ResourceManager.h>

#include <Engine/Resource/Model3D/Model3D.h>
#include <Engine/Resource/Model3D/Skeleton.h>
#include <Engine/Resource/Model3D/FBXLoader.h>
#include <Engine/Resource/Model3D/Animation3D.h>
#include <Engine/Resource/Shader/ComputeShaders/Animation3D_ComputeShader.h>

#include <Engine/Game/Component/Animator/Animation3D_PlayData.h>

namespace core
{
	Skeleton::Skeleton()
		: Resource()
		, m_bones{}
		, m_bone_offset_sbuffer{}
		, m_animations{}
		, m_final_matrix_buffer{}
	{
		m_compute_shader = LOAD_COMPUTESHADER(Animation3D_ComputeShader);
		ASSERT(nullptr != m_compute_shader, "컴퓨트쉐이더가 없음");

		StructBuffer::tDesc desc{};
		desc.m_SRV_target_register_idx = GPU::Register::t::g_FinalBoneMatrixArray;
		desc.m_SRV_target_stage = eShaderStageFlag::Vertex;
		desc.m_UAV_target_register_idx = GPU::Register::u::g_FinalBoneMatrixArrayRW;
		desc.m_buffer_RW_type = eStructBufferType::READ_WRITE;
		m_final_matrix_buffer = std::make_unique<StructBuffer>();
		m_final_matrix_buffer->create<MATRIX>(desc);
	}

	Skeleton::~Skeleton()
	{
	}

	eResult Skeleton::save(const std::fs::path& _base_dir) const
	{
		std::fs::path file_path = get_res_filename();

		//A/B.sklt 형식인지 확인한다.
		if (false == file_path.has_extension())
		{
			ERROR_MESSAGE("저장할 파일명이나 확장자가 없습니다.");
			return eResult::Fail;
		}
		if (false == file_path.has_parent_path())
		{
			ERROR_MESSAGE("Skeleton은 반드시 폴더 안에 저장되어야 합니다.");
			return eResult::Fail;
		}

		std::fs::path full_path = _base_dir / file_path;
		//Skeleton
		eResult result = save_file_binary(full_path);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("스켈레톤 정보 로드 실패.");
			return eResult::Fail;
		}
		
		//Animation3D는 Skeleton에 종속되므로 ResourceManager를 사용하지 않음 -> SaveFile_Binary을 통해 저장한다.
		{
			for (const auto& iter : m_animations)
			{
				full_path.replace_filename(iter.first);
				full_path.replace_extension(name::path::extension::Anim3D);
				result = iter.second->save_file_binary(full_path);
				if (eResult_fail(result))
				{
					std::wstringstream errmsg{};
					errmsg << L"애니메이션 저장 실패\b실패한 애니메이션 이름: ";
					errmsg << StringConverter::UTF8_to_UTF16(iter.first);
					ERROR_MESSAGE_W(errmsg.str().c_str());

					continue;
				}
			}
		}

		return result;
	}
	eResult Skeleton::load(const std::fs::path& _base_dir)
	{
		std::fs::path file_path = get_res_filename();

		//A/B.sklt 형식인지 확인한다.
		if (false == file_path.has_extension())
		{
			ERROR_MESSAGE("저장할 파일명이나 확장자가 없습니다.");
			return eResult::Fail;
		}
		if (false == file_path.has_parent_path())
		{
			ERROR_MESSAGE("Skeleton은 반드시 폴더 안에 저장되어야 합니다.");
			return eResult::Fail;
		}

		std::fs::path full_path = _base_dir / file_path;

		//Skeleton
		eResult result = load_file_binary(full_path);
		if (eResult_fail(result))
		{
			ERROR_MESSAGE("스켈레톤 정보 로드 실패.");
			return eResult::Fail;
		}

		//Animation 3D: Skeleton과 동일한 경로의 .a3d 파일들
		try
		{
			//파일명 제거
			full_path.remove_filename();

			if (false == std::fs::is_directory(full_path))
			{
				ERROR_MESSAGE("경로가 잘못되었습니다.");
				return eResult::Fail_Open;
			}

			//내부 순회돌아주면서 a3d 확장자 파일을 모두 불러온다.
			for (const auto& dirIter : std::fs::directory_iterator(full_path))
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
				
				s_ptr<Animation3D> a3d = std::make_shared<Animation3D>();
				s_ptr<Skeleton> ths = std::static_pointer_cast<Skeleton>(shared_from_this());
				a3d->set_skeleton(ths);
				eResult result = a3d->load_file_binary(filePath);
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

	eResult Skeleton::serialize_binary(BinarySerializer& _ser) const
	{
		eResult result = Super::serialize_binary(_ser);
		if (eResult_fail(result)) { return result; }

		_ser << m_bones.size();
		for (size_t i = 0; i < m_bones.size(); ++i)
		{
			//std::string			strBoneName{};
			//int					iDepth;
			//int					iParentIndx;
			//MATRIX				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
			//MATRIX				matBone;	// 이거 안씀
			_ser << m_bones[i].name;
			_ser << m_bones[i].depth_level;
			_ser << m_bones[i].parent_idx;
			_ser << m_bones[i].offset_matrix;
			_ser << m_bones[i].bone_matrix;
		}

		return eResult::Success;
	}

	eResult Skeleton::deserialize_binary(const BinarySerializer& _ser)
	{
		eResult result = Super::deserialize_binary(_ser);
		if (eResult_fail(result)) { return result; }

		size_t size{};
		_ser >> size;
		m_bones.resize(size);
		for (size_t i = 0; i < m_bones.size(); ++i)
		{
			//std::string			strBoneName{};
			//int					iDepth;
			//int					iParentIndx;
			//MATRIX				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
			//MATRIX				matBone;	// 이거 안씀
			_ser >> m_bones[i].name;
			_ser >> m_bones[i].depth_level;
			_ser >> m_bones[i].parent_idx;
			_ser >> m_bones[i].offset_matrix;
			_ser >> m_bones[i].bone_matrix;
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
		m_bones.reserve(vecBone.size());
		for (UINT i = 0; i < vecBone.size(); ++i)
		{
			m_bones.push_back(tMTBone{});
			tMTBone& bone = m_bones.back();

			bone.depth_level = vecBone[i].depth_level;
			bone.parent_idx = vecBone[i].parent_bone_idx;
			bone.bone_matrix = FBXLoader::get_matrix_from_FbxAMatrix(vecBone[i].bone_matrix);
			bone.offset_matrix = FBXLoader::get_matrix_from_FbxAMatrix(vecBone[i].offset_matrix);
			bone.name = vecBone[i].bone_name;
		}
		create_bone_offset_buffer();

		const auto& animClip = _fbxLoader->get_animations();
		for (size_t i = 0; i < animClip.size(); ++i)
		{
			std::unique_ptr<Animation3D> anim = std::make_unique<Animation3D>();

			s_ptr<Skeleton> sklt =
				std::static_pointer_cast<Skeleton>(shared_from_this());

			eResult result = anim->load_from_fbx(sklt, &animClip[i]);

			if (eResult_fail(result))
			{
				ERROR_MESSAGE("애니메이션 생성 실패");
				return result;
			}
			
			std::string animName(anim->get_res_filename());
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
		if (m_bones.size() != _other.m_bones.size())
		{
			ERROR_MESSAGE("본 정보가 일치하지 않습니다.");
			return false;
		}
			

		std::vector<int> matchingIndices(m_bones.size());
		for (size_t i = 0; i < m_bones.size(); ++i)
		{
			//내 본과 일치하는 본의 인덱스를 찾는다.
			int otherIdx = _other.find_same_bone_index(m_bones[i]);

			//일치하는 본이 없으면 Return
			if (0 > otherIdx)
			{
				ERROR_MESSAGE("본 인덱스 정보가 일치하지 않습니다.");
				return false;
			}
				

			//내 본의 i번쨰 = 상대 본의 otherIdx 번쨰
			matchingIndices[i] = otherIdx;
		}

		for (const auto& otherAnim : _other.m_animations)
		{
			//일단 다른쪽의 애니메이션을 복사해온다.
			auto cloned = otherAnim.second->clone();
			ASSERT_DEBUG(cloned, "nullptr 에러");
			s_ptr<Animation3D> ourAnim = std::static_pointer_cast<Animation3D>(cloned);

			//스켈레톤 주소를 자신의것으로 변경
			s_ptr<Skeleton> my_shared_ptr =
				std::static_pointer_cast<Skeleton>(this->shared_from_this());
			ourAnim->set_skeleton(my_shared_ptr);

			//위에서 찾은 일치하는 Bone 번호 인덱스를 가져온다.
			for (size_t i = 0; i < matchingIndices.size(); ++i)
			{
				//인덱스가 서로 다른 경우에 인덱스 번호를 바꿔준다
				if ((int)i != matchingIndices[i])
				{
					std::swap(ourAnim->m_key_frames_per_bone[i], ourAnim->m_key_frames_per_bone[matchingIndices[i]]);
				}
			}

			std::string name = otherAnim.first;
			auto iter = m_animations.find(name);
			while (iter != m_animations.end())
			{
				name += "+";
				iter = m_animations.find(name);
			}

			std::fs::path full_path = ResourceManager<Skeleton>::get_inst().get_base_directory(); 
			full_path /= _saveDir.filename();
			full_path /= name;
			full_path.replace_extension(name::path::extension::Anim3D);
			
			if (eResult_fail(ourAnim->save_file_binary(full_path)))
			{
				ERROR_MESSAGE("Animation 3D 저장 실패");
				return false;
			}

			//우리 애니메이션 쪽에 등록
			m_animations.insert(std::make_pair(name, ourAnim));
		}

		return true;
	}

	int Skeleton::find_same_bone_index(const tMTBone& _other) const
	{
		for (size_t i = 0; i < m_bones.size(); ++i)
		{
			if (
				m_bones[i].name == _other.name
				&&
				m_bones[i].depth_level == _other.depth_level
				&&
				m_bones[i].parent_idx == _other.parent_idx
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
		vecOffset.reserve(m_bones.size());
		for (size_t i = 0; i < m_bones.size(); ++i)
		{
			vecOffset.push_back(m_bones[i].offset_matrix);
		}

		//CreateBuffer
		StructBuffer::tDesc desc{};
		desc.m_buffer_RW_type = eStructBufferType::READ_ONLY;
		desc.m_SRV_target_register_idx = GPU::Register::t::g_BoneOffsetArray;
		m_bone_offset_sbuffer = std::make_shared<StructBuffer>();

		eResult result = m_bone_offset_sbuffer->create<MATRIX>(desc, (UINT)vecOffset.size(), vecOffset.data(), (UINT)vecOffset.size());
		
		ASSERT_DEBUG(eResult_success(result), "본 오프셋 버퍼 생성 실패");
	}

	s_ptr<Animation3D> Skeleton::find_animation(const std::string_view _strAnimName)
	{
		s_ptr<Animation3D> retPtr = nullptr;
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
		uint size = (uint)(m_bones.size() * m_compute_queue.size());
		m_final_matrix_buffer->resize(size);

		//UAV에 바인딩한 뒤
		m_final_matrix_buffer->bind_buffer_as_UAV();

		//각자 애니메이션에 대해 업데이트를 수행
		for (uint i = 0; i < (uint)m_compute_queue.size(); ++i) {
			Animation3D_ComputeShader::tDesc desc{};

			desc.current_animation_key_frame_buffer = m_compute_queue[i]->get_current_animation()->get_keyframe_sbuffer().get();

			s_ptr<Animation3D> next_anim = m_compute_queue[i]->get_next_animation();
			if (next_anim)
			{
				desc.next_animation_keyframe_buffer = next_anim->get_keyframe_sbuffer().get();
			}

			desc.bone_offset_matrix_buffer = m_bone_offset_sbuffer.get();

			desc.final_bone_translation_matrix_buffer = m_final_matrix_buffer.get();

			desc.shared_animation_data = &(m_compute_queue[i]->get_compute_shader_data());

			m_compute_shader->on_execute(desc);
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

