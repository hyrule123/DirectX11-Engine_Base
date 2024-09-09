#include "Engine/Resource/Model3D/Animation3D.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/Manager/ResourceManager.h"

#include "Engine/Util/Serialize/BinarySerializer.h"

#include "Engine/Resource/Model3D/FBXLoader.h"
#include "Engine/Resource/Model3D/Model3D.h"
#include "Engine/Resource/Model3D/Skeleton.h"


namespace ehw
{
	Animation3D::Animation3D()
        : Animation(Animation3D::concrete_name)
        , m_OwnerSkeleton{}
        , m_StartFrame{}
        , m_EndFrame{}
        , m_FrameLength{}
        , m_StartTime{}
        , m_EndTime{}
        , m_TimeLength{}
        , m_UpdateTime{}
        , m_FramePerSec{}
        , m_KeyFramesPerBone{}
        , m_SBufferKeyFrame{}
	{
	}

    Animation3D::Animation3D(const Animation3D& _other)
        : Animation(_other)
        , m_OwnerSkeleton{ _other.m_OwnerSkeleton }
        , m_StartFrame{ _other.m_StartFrame }
        , m_EndFrame{ _other.m_EndFrame}
        , m_FrameLength{ _other.m_FrameLength }
        , m_StartTime{ _other.m_StartTime }
        , m_EndTime{ _other.m_EndTime }
        , m_TimeLength{ _other.m_TimeLength }
        , m_UpdateTime{_other.m_UpdateTime}
        , m_FramePerSec{_other.m_FramePerSec}
        , m_KeyFramesPerBone{_other.m_KeyFramesPerBone}
        , m_SBufferKeyFrame{}
    {
        m_SBufferKeyFrame = std::shared_ptr<StructBuffer>(_other.m_SBufferKeyFrame->Clone());
    }

    Animation3D::~Animation3D()
	{
	}

    void Animation3D::bind_data()
    {
        ASSERT(m_SBufferKeyFrame, "키프레임 버퍼가 존재하지 않습니다.");
        m_SBufferKeyFrame->BindDataSRV();
    }
    void Animation3D::UnBindData()
    {
        m_SBufferKeyFrame->unbind_data();
    }

    eResult Animation3D::save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const
    {
        ERROR_MESSAGE("Skeleton에서 SaveFile 함수를 통해 저장되는 방식입니다.");
        return eResult::Fail_NotImplemented;
    }

    eResult Animation3D::load(const std::fs::path& _baseDir, const std::fs::path& _key_path)
    {
        ERROR_MESSAGE("Skeleton에서 SaveFile 함수를 통해 저장되는 방식입니다.");
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

        ser << m_StartFrame;
        ser << m_EndFrame;
        ser << m_FrameLength;
        ser << m_StartTime;
        ser << m_EndTime;
        ser << m_TimeLength;
        ser << m_UpdateTime;
        ser << m_FramePerSec;

        ser << m_KeyFramesPerBone.size();
        for (size_t i = 0; i < m_KeyFramesPerBone.size(); ++i)
        {
            ser << m_KeyFramesPerBone[i].BoneIndex;
            ser << m_KeyFramesPerBone[i].vecKeyFrame;
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

        ser >> m_StartFrame;
        ser >> m_EndFrame;
        ser >> m_FrameLength;
        ser >> m_StartTime;
        ser >> m_EndTime;
        ser >> m_TimeLength;
        ser >> m_UpdateTime;
        ser >> m_FramePerSec;
        

        size_t vecSize = 0;
        ser >> vecSize;
        m_KeyFramesPerBone.resize(vecSize);
        for (size_t i = 0; i < m_KeyFramesPerBone.size(); ++i)
        {
            ser >> m_KeyFramesPerBone[i].BoneIndex;
            ser >> m_KeyFramesPerBone[i].vecKeyFrame;
        }

        if (false == CreateKeyFrameSBuffer())
        {
            ERROR_MESSAGE("키프레임 구조화 버퍼 생성 실패.");
            return eResult::Fail_Create;
        }

        return eResult::Success;
    }


	eResult Animation3D::LoadFromFBX(const std::shared_ptr<Skeleton>& _skeleton, const tFBXAnimClip* _clip)
	{
		if (nullptr == _skeleton || nullptr == _clip)
			return eResult::Fail_Nullptr;

        m_OwnerSkeleton = _skeleton;

        set_keypath(_clip->strName);

        switch (_clip->TimeMode)
        {
        case fbxsdk::FbxTime::eDefaultMode:
            m_FramePerSec = 30;
            break;
        case fbxsdk::FbxTime::eFrames120:
            m_FramePerSec = 120;
            break;
        case fbxsdk::FbxTime::eFrames100:
            m_FramePerSec = 100;
            break;
        case fbxsdk::FbxTime::eFrames60:
            m_FramePerSec = 60;
            break;
        case fbxsdk::FbxTime::eFrames50:
            m_FramePerSec = 50;
            break;
        case fbxsdk::FbxTime::eFrames48:
            m_FramePerSec = 48;
            break;
        case fbxsdk::FbxTime::eFrames30:
            [[fallthrough]];
        case fbxsdk::FbxTime::eFrames30Drop:
            [[fallthrough]];
        case fbxsdk::FbxTime::eNTSCDropFrame:
            [[fallthrough]];
        case fbxsdk::FbxTime::eNTSCFullFrame:
            m_FramePerSec = 30;
            break;
        case fbxsdk::FbxTime::ePAL:
            m_FramePerSec = 25;
            break;
        case fbxsdk::FbxTime::eFrames24:
            m_FramePerSec = 24;
            break;
        case fbxsdk::FbxTime::eFrames1000:
            m_FramePerSec = 1000;
            break;
        case fbxsdk::FbxTime::eCustom:
            m_FramePerSec = 30;
            break;
        case fbxsdk::FbxTime::eFrames96:
            m_FramePerSec = 96;
            break;
        case fbxsdk::FbxTime::eFrames72:
            m_FramePerSec = 72;
            break;
        default:
            m_FramePerSec = 60;
            break;
        }

        m_StartTime = _clip->StartTime.GetSecondDouble();
        if (m_StartTime < 0.0)
        {
            m_StartTime = 0.0;
        }
        m_EndTime = _clip->EndTime.GetSecondDouble();
        m_TimeLength = m_EndTime - m_StartTime;

        m_StartFrame = (int)_clip->StartTime.GetFrameCount(_clip->TimeMode);
        if (m_StartFrame < 0)
        {
            m_StartFrame = 0;
        }
        m_EndFrame = (int)_clip->EndTime.GetFrameCount(_clip->TimeMode);
        m_FrameLength = (int)(m_EndFrame - m_StartFrame + 1);//+1 -> 0프레임부터 시작이므로

        ASSERT(m_FrameLength >= 0, "3D 애니메이션의 프레임이 음수입니다.");


        //본의 사이즈 * 프레임 수 만큼 사이즈를 설정
        //그러므로 CPU 메모리에서는 이중 배열 형태로 저장해 놓고
        // 본 3개 키프레임 5개일 경우
        // 본0키0|본0키1|본0키2|본0키3|본0키4|
        // 본1키0|본1키1|본1키2|본1키3|본1키4|
        // 본2키0|본2키1|본2키2|본2키3|본2키4
        //그러므로 하나의 애니메이션은 본의 갯수 * 키프레임 갯수가 된다
        size_t boneCount = _clip->KeyFramesPerBone.size();
        m_KeyFramesPerBone.resize(boneCount);//CPU
        for (size_t i = 0; i < boneCount; ++i)
        {
            m_KeyFramesPerBone[i].BoneIndex = _clip->KeyFramesPerBone[i].BoneIndex;
            
            //m_KeyFramesPerBone[i].vecKeyFrame.resize(m_FrameLength);

            //GPU로 보낼 데이터 세팅
            //GPU는 이중 배열 같은걸 지원 안함
            //GPU 메모리는 이걸 1차원 배열 형태로 펴서 데이터를 보내준다
            //그러니까 1차원 배열 형태로 애니메이션 데이터를 보내야 함
            
            //FBX가 왜인지는 모르겠지만 키프레임이 비정상적으로 로딩되는 경우가 있어서 
            //키프레임 갯수는 고정된 프레임 수로 생성해준다
            //본의 사이즈 * 프레임 수(2차 배열 -> 1차 배열)


            //애니메이션 전체 키프레임의 갯수와, 실제 애니메이션의 중 적은 쪽의 것을 사용해준다
            //이유: 키프레임이 0개일수도 있고, 등록된 키프레임 수보다 훨씬 많을수도 있음
            size_t resizeCount =
                m_FrameLength <= _clip->KeyFramesPerBone[i].vecKeyFrame.size() ?
                m_FrameLength : _clip->KeyFramesPerBone[i].vecKeyFrame.size();

            m_KeyFramesPerBone[i].vecKeyFrame.resize(resizeCount);
            for(size_t j = 0; j < m_KeyFramesPerBone[i].vecKeyFrame.size(); ++j)
            {
                if ((int)j < m_FrameLength)
                {
                    //우리 포맷 키프레임
                    tKeyFrame& projKeyFrame = m_KeyFramesPerBone[i].vecKeyFrame[j];

                    //FBX 포맷 키프레임
                    const tFBXKeyFrame& fbxKeyFrame = _clip->KeyFramesPerBone[i].vecKeyFrame[j];

                    //포맷을 변환해줘야 한다. double -> float

                    const fbxsdk::FbxVector4& fbxPos = fbxKeyFrame.matTransform.GetT();
                    projKeyFrame.Trans.Pos.x = (float)fbxPos.mData[0];
                    projKeyFrame.Trans.Pos.y = (float)fbxPos.mData[1];
                    projKeyFrame.Trans.Pos.z = (float)fbxPos.mData[2];
                    projKeyFrame.Trans.Pos.w = 1.f;

                    const fbxsdk::FbxVector4& fbxScale = fbxKeyFrame.matTransform.GetS();
                    projKeyFrame.Trans.Scale.x = (float)fbxScale.mData[0];
                    projKeyFrame.Trans.Scale.y = (float)fbxScale.mData[1];
                    projKeyFrame.Trans.Scale.z = (float)fbxScale.mData[2];
                    projKeyFrame.Trans.Pos.w = 1.f;

                    const fbxsdk::FbxQuaternion& fbxQuat = fbxKeyFrame.matTransform.GetQ();
                    projKeyFrame.Trans.RotQuat.x = (float)fbxQuat.mData[0];
                    projKeyFrame.Trans.RotQuat.y = (float)fbxQuat.mData[1];
                    projKeyFrame.Trans.RotQuat.z = (float)fbxQuat.mData[2];
                    projKeyFrame.Trans.RotQuat.w = (float)fbxQuat.mData[3];
                }
            }
        }

        if (false == CreateKeyFrameSBuffer())
        {
            return eResult::Fail_InValid;
        }

		return eResult::Success;
	}
#pragma endregion

    bool Animation3D::CreateKeyFrameSBuffer()
    {
        ASSERT(false == m_KeyFramesPerBone.empty(), "애니메이션 프레임 정보가 비어 있습니다.");

        std::vector<tAnimKeyframeTranslation>	vecFrameTrans;//GPU

        //Bone의 갯수 * Frame의 수가 전체 배열의 갯수가 된다.
        vecFrameTrans.resize(m_KeyFramesPerBone.size() * m_FrameLength);

        for (size_t i = 0; i < m_KeyFramesPerBone.size(); ++i)
        {
            for (size_t j = 0; j < m_KeyFramesPerBone[i].vecKeyFrame.size(); ++j)
            {
                //i번째 본의 j 키프레임 데이터
                size_t indexIn1DArray = m_FrameLength * i + j;
                vecFrameTrans[indexIn1DArray] = m_KeyFramesPerBone[i].vecKeyFrame[j].Trans;
            }
        }


        StructBuffer::Desc desc{};
        desc.eSBufferType = eStructBufferType::READ_ONLY;
        desc.GPU_register_t_SRV = GPU::Register::t::g_FrameTransArray;
        desc.TargetStageSRV = eShaderStageFlag::Compute;
        m_SBufferKeyFrame = std::make_shared<StructBuffer>();

        eResult result = 
            m_SBufferKeyFrame->Init<tAnimKeyframeTranslation>(desc, (uint)vecFrameTrans.size(), vecFrameTrans.data(), (uint)vecFrameTrans.size());
        if (eResult_fail(result)) {
            return false;
        }

        return true;
    }
}
