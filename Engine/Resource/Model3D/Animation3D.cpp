#include "Animation3D.h"

#include "../../GPU/StructBuffer.h"

#include "../../Util/Serializer.h"

#include "../../Manager/ResourceManager.h"


#include "FBXLoader.h"
#include "Model3D.h"
#include "Skeleton.h"


namespace ehw
{
	Animation3D::Animation3D()
        : iAnimation(typeid(Animation3D))
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

    Animation3D::~Animation3D()
	{
	}

    void Animation3D::BindData()
    {
        ASSERT(m_SBufferKeyFrame, "키프레임 버퍼가 존재하지 않습니다.");
        m_SBufferKeyFrame->BindDataSRV();
    }
    void Animation3D::UnBindData()
    {
        m_SBufferKeyFrame->UnBindData();
    }

#pragma region 저장 및 불러오기
    eResult Animation3D::Save(const std::fs::path& _pathFromBaseDir)
    {
        if (nullptr == m_OwnerSkeleton)
        {
            ERROR_MESSAGE("본 정보가 존재하지 않습니다.");
            return eResult::Fail_Nullptr;
        }

        SetStrKey(_pathFromBaseDir.string());

        if (false == _pathFromBaseDir.has_parent_path())
        {
            ERROR_MESSAGE("Animation은 반드시 상위 폴더가 있어야 합니다.");
            return eResult::Fail_InValid;
        }

        std::fs::path fullPath = ResourceManager<Model3D>::GetBaseDir() / _pathFromBaseDir;

        //디렉토리 존재여부 확인 및 없을 시 생성
        {
            std::fs::path checkDir = fullPath.parent_path();
            if (false == std::fs::exists(checkDir))
            {
                std::fs::create_directories(checkDir);
            }
        }

        fullPath.replace_extension(strKey::path::extension::Anim3D);

        std::ofstream ofs("test");

        BinarySerializer binSave{};
        binSave << m_StartFrame;
        binSave << m_EndFrame;
        binSave << m_FrameLength;
        binSave << m_StartTime;
        binSave << m_EndTime;
        binSave << m_TimeLength;
        binSave << m_UpdateTime;
        binSave << m_FramePerSec;

        binSave << m_KeyFramesPerBone.size();
        for (size_t i = 0; i < m_KeyFramesPerBone.size(); ++i)
        {
            binSave << m_KeyFramesPerBone[i].BoneIndex;
            binSave << m_KeyFramesPerBone[i].vecKeyFrame;
        }

        binSave.Save(fullPath);

        return eResult::Success;
    }

    eResult Animation3D::Load(const std::fs::path& _pathFromBaseDir)
    {
        SetStrKey(_pathFromBaseDir.string());

        if (false == _pathFromBaseDir.has_parent_path())
        {
            ERROR_MESSAGE("Animation은 반드시 상위 폴더가 있어야 합니다.");
            return eResult::Fail_InValid;
        }
        std::fs::path fullPath = ResourceManager<Model3D>::GetBaseDir() / _pathFromBaseDir;
        fullPath.replace_extension(strKey::path::extension::Anim3D);

        if (false == std::fs::exists(fullPath))
        {
            ERROR_MESSAGE("파일이 존재하지 않습니다.");
            return eResult::Fail_Open;
        }

        BinarySerializer binLoad{};

        if (false == binLoad.Load(fullPath))
        {
            return eResult::Fail_Open;
        }

        binLoad >> m_StartFrame;
        binLoad >> m_EndFrame;
        binLoad >> m_FrameLength;
        binLoad >> m_StartTime;
        binLoad >> m_EndTime;
        binLoad >> m_TimeLength;
        binLoad >> m_UpdateTime;
        binLoad >> m_FramePerSec;

        size_t vecSize = 0;
        binLoad >> vecSize;
        m_KeyFramesPerBone.resize(vecSize);
        for (size_t i = 0; i < m_KeyFramesPerBone.size(); ++i)
        {
            binLoad >> m_KeyFramesPerBone[i].BoneIndex;
            binLoad >> m_KeyFramesPerBone[i].vecKeyFrame;
        }

        std::vector<tAnimKeyframeTranslation>	vecFrameTrans;
        vecFrameTrans.resize(m_OwnerSkeleton->GetBoneCount() * m_FrameLength);
        {
            size_t KeyFramesPerBoneSize{};
            binLoad >> KeyFramesPerBoneSize;
            m_KeyFramesPerBone.resize(KeyFramesPerBoneSize);
            for (size_t i = 0; i < m_KeyFramesPerBone.size(); ++i)
            {
                binLoad >> m_KeyFramesPerBone[i].BoneIndex;
                binLoad >> m_KeyFramesPerBone[i].vecKeyFrame;

                for (size_t j = 0; j < m_KeyFramesPerBone[i].vecKeyFrame.size(); ++j)
                {
                    size_t indexIn1DArray = m_FrameLength * i + j;
                    vecFrameTrans[indexIn1DArray] = m_KeyFramesPerBone[i].vecKeyFrame[j].Trans;
                }
            }
        }
        
        if (false == CreateKeyFrameSBuffer(vecFrameTrans))
        {
            ERROR_MESSAGE("키프레임 구조화 버퍼 생성 실패.");
            return eResult::Fail_Create;
        }
       
        return eResult::Success;
    }

	eResult Animation3D::LoadFromFBX(Skeleton* _skeleton, const tFBXAnimClip* _clip)
	{
		if (nullptr == _skeleton || nullptr == _clip)
			return eResult::Fail_Nullptr;

        m_OwnerSkeleton = _skeleton;

        SetStrKey(_clip->strName);

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
        std::vector<tAnimKeyframeTranslation>	vecFrameTrans;//GPU
        size_t boneCount = _clip->KeyFramesPerBone.size();
        vecFrameTrans.resize(boneCount * m_FrameLength);
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



                    //i번째 본의 j 키프레임 데이터
                    size_t indexIn1DArray = m_FrameLength * i + j;
                    vecFrameTrans[indexIn1DArray] = projKeyFrame.Trans;
                    //vecFrameTrans[indexIn1DArray].vTranslate =  float4(projKeyFrame.Pos, 1.f);
                    //vecFrameTrans[indexIn1DArray].Scale =      float4(projKeyFrame.Scale, 1.f);
                    //vecFrameTrans[indexIn1DArray].RotQuat =        projKeyFrame.RotQuat;
                } 
 
            }
        }

        if (false == CreateKeyFrameSBuffer(vecFrameTrans))
        {
            return eResult::Fail_InValid;
        }

		return eResult::Success;
	}
#pragma endregion

    bool Animation3D::CreateKeyFrameSBuffer(const std::vector<tAnimKeyframeTranslation>& _vecAnimFrameTranslations)
    {
        ASSERT(false == _vecAnimFrameTranslations.empty(), "애니메이션 프레임 정보가 비어 있습니다.");

        StructBuffer::Desc desc{};
        desc.eSBufferType = eStructBufferType::READ_ONLY;
        desc.REGISLOT_t_SRV = Register_t_g_FrameTransArray;
        desc.TargetStageSRV = eShaderStageFlag::Compute;
        m_SBufferKeyFrame = std::make_unique<StructBuffer>(desc);

        if (FAILED(m_SBufferKeyFrame->Create<tAnimKeyframeTranslation>
            (_vecAnimFrameTranslations.size(), _vecAnimFrameTranslations.data(), _vecAnimFrameTranslations.size())))
        {
            return false;
        }

        return true;
    }
}
