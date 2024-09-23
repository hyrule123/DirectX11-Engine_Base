#pragma once
#include "Engine/Resource/Animation.h"

#include "Engine/Util/SimpleMath.h"
#include "Engine/Util/Serialize/Serializable.h"

#include "Engine/Common.h"

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
        : public Animation
        , public Serializable_Binary
	{
        CLASS_NAME(Animation3D);
        REGISTER_INSTANCE_DEFAULT(Animation3D);
        
        friend class Skeleton;
	public:
		Animation3D();

        Animation3D(const Animation3D& _other);
        CLONE_ABLE(Animation3D);

        virtual ~Animation3D();

    public:
        void bind_buffer_to_GPU_register();
        void unbind_buffer_from_GPU_register();

        virtual eResult save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const override;
        virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

        eResult serialize_binary(BinarySerializer* _ser) const override;
        eResult deserialize_binary(const BinarySerializer* _ser) override;

        eResult load_from_fbx(const std::shared_ptr<Skeleton>& _skeleton, const tFBXAnimClip* _clip);

        uint get_start_frame() const { return m_StartFrame; }
        uint GetEndFrame() const { return m_EndFrame; }
        uint GetFrameLength() const { return m_FrameLength; }
        double GetStartTime() const { return m_StartTime; }
        double GetEndTIme() const { return m_EndTime; }
        double GetTimeLength() const { return m_TimeLength; }
        float GetUpdateTime() const { return m_UpdateTime; }
        uint GetFPS() const { return m_FramePerSec; }
        std::shared_ptr<Skeleton> get_skeleton() const { return m_OwnerSkeleton.lock(); }
        void set_skeleton(const std::shared_ptr<Skeleton>& _skeleton) { m_OwnerSkeleton = _skeleton; }
        std::shared_ptr<StructBuffer> GetKeyFrameSBuffer() const { return m_SBufferKeyFrame; }

    private:
        bool CreateKeyFrameSBuffer();

    private:
        std::weak_ptr<Skeleton> m_OwnerSkeleton;

        uint				m_StartFrame;
        uint				m_EndFrame;
        uint				m_FrameLength;

        double			m_StartTime;
        double			m_EndTime;
        double			m_TimeLength;
        float			m_UpdateTime; // 이거 안씀

        uint         	m_FramePerSec;

        //이중 배열 형태임
        std::vector<tKeyFramesPerBone>          m_KeyFramesPerBone;
        std::shared_ptr<StructBuffer>			m_SBufferKeyFrame;
	};
}


