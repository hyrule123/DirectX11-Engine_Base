#pragma once
#include "../iAnimation.h"



#include "../../Util/SimpleMath.h"
#include "../../Util/Serialize/BinarySerializer.h"
#include "../../defines.h"


namespace ehw
{
    struct tKeyFrame
    {
        double              Time{};
        tAnimKeyframeTranslation Trans{};
        //float3              Scale{};
        //math::Quaternion    RotQuat{};
        //float3              Pos{};
    };

    struct tKeyFramesPerBone
    {
        int                         BoneIndex{};
        std::vector<tKeyFrame>     vecKeyFrame{};
    };


    struct tFBXAnimClip;
    class StructBuffer;
    class Skeleton;
	class Animation3D final
        : public iAnimation
        , public Serializable<BinarySerializer>
	{
        friend class Skeleton;
	public:
		Animation3D();

        Animation3D(const Animation3D& _other);
        CLONE(Animation3D);

        virtual ~Animation3D();

    public:
        void BindData();
        void UnBindData();

        virtual eResult Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;
        virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;

        eResult Serialize(BinarySerializer& _ser) override;
        eResult DeSerialize(BinarySerializer& _ser) override;

        eResult LoadFromFBX(const std::shared_ptr<Skeleton>& _skeleton, const tFBXAnimClip* _clip);

        int GetStartFrame() const { return m_StartFrame; }
        int GetEndFrame() const { return m_EndFrame; }
        int GetFrameLength() const { return m_FrameLength; }
        double GetStartTime() const { return m_StartTime; }
        double GetEndTIme() const { return m_EndTime; }
        double GetTimeLength() const { return m_TimeLength; }
        float GetUpdateTime() const { return m_UpdateTime; }
        int GetFPS() const { return m_FramePerSec; }
        std::shared_ptr<Skeleton> GetSkeleton() const { return m_OwnerSkeleton.lock(); }
        void SetSkeleton(const std::shared_ptr<Skeleton>& _skeleton) { m_OwnerSkeleton = _skeleton; }
        StructBuffer* GetKeyFrameSBuffer() const { return m_SBufferKeyFrame.get(); }

    private:
        bool CreateKeyFrameSBuffer();

    private:
        std::weak_ptr<Skeleton> m_OwnerSkeleton;

        int				m_StartFrame;
        int				m_EndFrame;
        int				m_FrameLength;

        double			m_StartTime;
        double			m_EndTime;
        double			m_TimeLength;
        float			m_UpdateTime; // 이거 안씀

        int         	m_FramePerSec;



        //이중 배열 형태임
        std::vector<tKeyFramesPerBone>          m_KeyFramesPerBone;
        std::shared_ptr<StructBuffer>			m_SBufferKeyFrame;
	};
}


