
#include "Animation3D.h"

#include <Engine/GPU/StructBuffer.h>

#include <Engine/Manager/ResourceManager.h>

#include <Engine/Util/Serialize/BinarySerializer.h>

#include <Engine/Resource/Model3D/FBXLoader.h>
#include <Engine/Resource/Model3D/Model3D.h>
#include <Engine/Resource/Model3D/Skeleton.h>

namespace core
{
	Animation3D::Animation3D()
        : Animation(Animation3D::s_static_type_name)
        , m_OwnerSkeleton{}
        , m_start_frame_idx{}
        , m_end_frame_idx{}
        , m_frame_length()
        , m_start_time{}
        , m_end_time{}
        , m_time_length{}
        , m_update_time{}
        , m_frame_per_sec{}
        , m_key_frames_per_bone{}
        , m_keyframe_sbuffer{}
    {
	}

    Animation3D::Animation3D(const Animation3D& _other)
        : Animation(_other)
        , m_OwnerSkeleton{ _other.m_OwnerSkeleton }
        , m_start_frame_idx{ _other.m_start_frame_idx }
        , m_end_frame_idx{ _other.m_end_frame_idx}
        , m_frame_length(_other.m_frame_length)
        , m_start_time{ _other.m_start_time }
        , m_end_time{ _other.m_end_time }
        , m_time_length{ _other.m_time_length }
        , m_update_time{_other.m_update_time}
        , m_frame_per_sec{_other.m_frame_per_sec}
        , m_key_frames_per_bone{_other.m_key_frames_per_bone}
        , m_keyframe_sbuffer{}
        
    {
        m_keyframe_sbuffer = std::static_pointer_cast<StructBuffer>(_other.m_keyframe_sbuffer->clone());
    }

    Animation3D::~Animation3D()
	{
	}

    void Animation3D::bind_buffer_to_GPU_register()
    {
        ASSERT(m_keyframe_sbuffer, "키프레임 버퍼가 존재하지 않습니다.");
        m_keyframe_sbuffer->bind_buffer_as_SRV();
    }
    void Animation3D::unbind_buffer_from_GPU_register()
    {
        m_keyframe_sbuffer->unbind_buffer();
    }

    eResult Animation3D::save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const
    {
        ERROR_MESSAGE("Skeleton에서 save_file 함수를 통해 저장되는 방식입니다.");
        return eResult::Fail_NotImplemented;
    }

    eResult Animation3D::load(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
    {
        ERROR_MESSAGE("Skeleton에서 save_file 함수를 통해 저장되는 방식입니다.");
        return eResult::Fail_NotImplemented;
    }


    eResult Animation3D::serialize_binary(BinarySerializer* _ser) const
    {
        if (nullptr == _ser)
        {
            ERROR_MESSAGE("Serializer이 nullptr 입니다.");
            return eResult::Fail_Nullptr;
        }
        if (m_OwnerSkeleton.expired())
        {
            ERROR_MESSAGE("본 정보가 존재하지 않습니다.");
            return eResult::Fail_Nullptr;
        }

        BinarySerializer& ser = *_ser;

        ser << m_start_frame_idx;
        ser << m_end_frame_idx;
        ser << m_frame_length;
        ser << m_start_time;
        ser << m_end_time;
        ser << m_time_length;
        ser << m_update_time;
        ser << m_frame_per_sec;

        ser << m_key_frames_per_bone.size();
        for (size_t i = 0; i < m_key_frames_per_bone.size(); ++i)
        {
            ser << m_key_frames_per_bone[i].bone_idx;
            ser << m_key_frames_per_bone[i].key_frames;
        }

        return eResult::Success;
    }

    eResult Animation3D::deserialize_binary(const BinarySerializer* _ser)
    {
        if (nullptr == _ser)
        {
            ERROR_MESSAGE("Serializer이 nullptr 입니다.");
            return eResult::Fail_Nullptr;
        }

        const BinarySerializer& ser = *_ser;

        ser >> m_start_frame_idx;
        ser >> m_end_frame_idx;
        ser >> m_frame_length;
        ser >> m_start_time;
        ser >> m_end_time;
        ser >> m_time_length;
        ser >> m_update_time;
        ser >> m_frame_per_sec;

        size_t vecSize = 0;
        ser >> vecSize;
        m_key_frames_per_bone.resize(vecSize);
        for (size_t i = 0; i < m_key_frames_per_bone.size(); ++i)
        {
            ser >> m_key_frames_per_bone[i].bone_idx;
            ser >> m_key_frames_per_bone[i].key_frames;
        }

        if (false == create_keyframe_sbuffer())
        {
            ERROR_MESSAGE("키프레임 구조화 버퍼 생성 실패.");
            return eResult::Fail_Create;
        }

        return eResult::Success;
    }


	eResult Animation3D::load_from_fbx(const s_ptr<Skeleton>& _skeleton, const tFBXAnimClip* _clip)
	{
		if (nullptr == _skeleton || nullptr == _clip)
			return eResult::Fail_Nullptr;

        m_OwnerSkeleton = _skeleton;

        set_resource_name(_clip->name);

        switch (_clip->time_mode)
        {
        case fbxsdk::FbxTime::eDefaultMode:
            m_frame_per_sec = 30;
            break;
        case fbxsdk::FbxTime::eFrames120:
            m_frame_per_sec = 120;
            break;
        case fbxsdk::FbxTime::eFrames100:
            m_frame_per_sec = 100;
            break;
        case fbxsdk::FbxTime::eFrames60:
            m_frame_per_sec = 60;
            break;
        case fbxsdk::FbxTime::eFrames50:
            m_frame_per_sec = 50;
            break;
        case fbxsdk::FbxTime::eFrames48:
            m_frame_per_sec = 48;
            break;
        case fbxsdk::FbxTime::eFrames30:
            [[fallthrough]];
        case fbxsdk::FbxTime::eFrames30Drop:
            [[fallthrough]];
        case fbxsdk::FbxTime::eNTSCDropFrame:
            [[fallthrough]];
        case fbxsdk::FbxTime::eNTSCFullFrame:
            m_frame_per_sec = 30;
            break;
        case fbxsdk::FbxTime::ePAL:
            m_frame_per_sec = 25;
            break;
        case fbxsdk::FbxTime::eFrames24:
            m_frame_per_sec = 24;
            break;
        case fbxsdk::FbxTime::eFrames1000:
            m_frame_per_sec = 1000;
            break;
        case fbxsdk::FbxTime::eCustom:
            m_frame_per_sec = 30;
            break;
        case fbxsdk::FbxTime::eFrames96:
            m_frame_per_sec = 96;
            break;
        case fbxsdk::FbxTime::eFrames72:
            m_frame_per_sec = 72;
            break;
        default:
            m_frame_per_sec = 60;
            break;
        }

        m_start_time = _clip->start_time.GetSecondDouble();
        if (m_start_time < 0.0)
        {
            m_start_time = 0.0;
        }
        m_end_time = _clip->end_time.GetSecondDouble();
        m_time_length = m_end_time - m_start_time;

        m_start_frame_idx = (uint)_clip->start_frame_idx;
        m_end_frame_idx = (uint)_clip->end_frame_idx;

        ASSERT(m_start_frame_idx <= m_end_frame_idx, "3D 애니메이션의 프레임이 음수입니다.");
        m_frame_length = (uint)_clip->frame_length;

        //본의 사이즈 * 프레임 수 만큼 사이즈를 설정
        //그러므로 CPU 메모리에서는 이중 배열 형태로 저장해 놓고
        // 본 3개 키프레임 5개일 경우
        // 본0키0|본0키1|본0키2|본0키3|본0키4|
        // 본1키0|본1키1|본1키2|본1키3|본1키4|
        // 본2키0|본2키1|본2키2|본2키3|본2키4
        //그러므로 하나의 애니메이션은 본의 갯수 * 키프레임 갯수가 된다
        size_t boneCount = _clip->key_frames_per_bone.size();
        m_key_frames_per_bone.resize(boneCount);//CPU

        for (size_t i = 0; i < boneCount; ++i)
        {
            m_key_frames_per_bone[i].bone_idx = _clip->key_frames_per_bone[i].bone_idx;
            
            //m_key_frames_per_bone[i].key_frames.resize(m_FrameLength);

            //GPU로 보낼 데이터 세팅
            //GPU는 이중 배열 같은걸 지원 안함
            //GPU 메모리는 이걸 1차원 배열 형태로 펴서 데이터를 보내준다
            //그러니까 1차원 배열 형태로 애니메이션 데이터를 보내야 함
            
            //FBX가 왜인지는 모르겠지만 키프레임이 비정상적으로 로딩되는 경우가 있어서 
            //키프레임 갯수는 고정된 프레임 수로 생성해준다
            //본의 사이즈 * 프레임 수(2차 배열 -> 1차 배열)

            //전부 동일하게 키프레임을 가지고 있음이 보장됨
            m_key_frames_per_bone[i].key_frames.resize(m_frame_length);
            for(uint j = 0; j < m_frame_length; ++j)
            {
                //우리 포맷 키프레임
                tKeyFrame& projKeyFrame = m_key_frames_per_bone[i].key_frames[j];

                //FBX 포맷 키프레임
                const tFBXKeyFrame& fbxKeyFrame = _clip->key_frames_per_bone[i].key_frames[j];

                //포맷을 변환해줘야 한다. double -> float
                projKeyFrame.time = (float)fbxKeyFrame.time;

                const fbxsdk::FbxVector4& fbxPos = fbxKeyFrame.transform_matrix.GetT();
                projKeyFrame.translation.Pos.x = (float)fbxPos.mData[0];
                projKeyFrame.translation.Pos.y = (float)fbxPos.mData[1];
                projKeyFrame.translation.Pos.z = (float)fbxPos.mData[2];
                projKeyFrame.translation.Pos.w = 1.f;

                const fbxsdk::FbxVector4& fbxScale = fbxKeyFrame.transform_matrix.GetS();
                projKeyFrame.translation.Scale.x = (float)fbxScale.mData[0];
                projKeyFrame.translation.Scale.y = (float)fbxScale.mData[1];
                projKeyFrame.translation.Scale.z = (float)fbxScale.mData[2];
                projKeyFrame.translation.Pos.w = 1.f;

                const fbxsdk::FbxQuaternion& fbxQuat = fbxKeyFrame.transform_matrix.GetQ();
                projKeyFrame.translation.RotQuat.x = (float)fbxQuat.mData[0];
                projKeyFrame.translation.RotQuat.y = (float)fbxQuat.mData[1];
                projKeyFrame.translation.RotQuat.z = (float)fbxQuat.mData[2];
                projKeyFrame.translation.RotQuat.w = (float)fbxQuat.mData[3];
            }
        }

        if (false == create_keyframe_sbuffer())
        {
            return eResult::Fail_InValid;
        }

		return eResult::Success;
	}
#pragma endregion

    bool Animation3D::create_keyframe_sbuffer()
    {
        ASSERT(false == m_key_frames_per_bone.empty(), "애니메이션 프레임 정보가 비어 있습니다.");

        std::vector<tAnimKeyframeTranslation>	vecFrameTrans;//GPU

        vecFrameTrans.resize(m_key_frames_per_bone.size() * (size_t)m_frame_length);

        for (size_t i = 0; i < m_key_frames_per_bone.size(); ++i)
        {
            for (size_t j = 0; j < m_key_frames_per_bone[i].key_frames.size(); ++j)
            {
                //i번째 본의 j 키프레임 데이터
                size_t indexIn1DArray = (m_end_frame_idx + 1) * i + j;
                vecFrameTrans[indexIn1DArray] = m_key_frames_per_bone[i].key_frames[j].translation;
            }
        }


        StructBuffer::tDesc desc{};
        desc.m_buffer_RW_type = eStructBufferType::READ_ONLY;
        desc.m_SRV_target_register_idx = GPU::Register::t::g_FrameTransArray;
        desc.m_SRV_target_stage = eShaderStageFlag::Compute;
        m_keyframe_sbuffer = std::make_shared<StructBuffer>();

        eResult result = 
            m_keyframe_sbuffer->create<tAnimKeyframeTranslation>(desc, (uint)vecFrameTrans.size(), vecFrameTrans.data(), (uint)vecFrameTrans.size());
        if (eResult_fail(result)) {
            return false;
        }

        return true;
    }
}
