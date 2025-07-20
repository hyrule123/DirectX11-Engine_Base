#pragma once
#include <Engine/Resource/Animation.h>

#include <Engine/Util/SimpleMath.h>
#include <Engine/Util/Serialize/Serializable.h>

#include <Engine/Common.h>

namespace core
{
    struct tKeyFrame
    {
        double              time{};
        tAnimKeyframeTranslation translation{};
        //float3              Scale{};
        //math::Quaternion    RotQuat{};
        //float3              Pos{};
    };

    struct tKeyFramesPerBone
    {
        int                         bone_idx{};
        std::vector<tKeyFrame>     key_frames{};
    };


    struct tFBXAnimClip;
    class StructBuffer;
    class Skeleton;
	class Animation3D final
        : public Animation
        , public Serializable_Binary
	{
        friend class Skeleton;

        CLASS_INFO(Animation3D, Animation);
        REGISTER_FACTORY(Animation3D);
        CLONE_ABLE(Animation3D);
        
	public:
		Animation3D();
        Animation3D(const Animation3D& _other);

        virtual ~Animation3D();

    public:
        void bind_buffer_to_GPU_register();
        void unbind_buffer_from_GPU_register();

        virtual eResult save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const override;
        virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

        eResult serialize_binary(BinarySerializer* _ser) const override;
        eResult deserialize_binary(const BinarySerializer* _ser) override;

        eResult load_from_fbx(const s_ptr<Skeleton>& _skeleton, const tFBXAnimClip* _clip);

        uint get_start_frame() const { return m_start_frame_idx; }
        uint get_end_frame() const { return m_end_frame_idx; }
        uint get_frame_length() const { return m_frame_length; }
        double get_start_time() const { return m_start_time; }
        double get_end_time() const { return m_end_time; }
        double get_time_length() const { return m_time_length; }
        float get_update_time() const { return m_update_time; }
        uint get_FPS() const { return m_frame_per_sec; }
        s_ptr<Skeleton> get_skeleton() const { return m_OwnerSkeleton.lock(); }
        void set_skeleton(const s_ptr<Skeleton>& _skeleton) { m_OwnerSkeleton = _skeleton; }
        s_ptr<StructBuffer> get_keyframe_sbuffer() const { return m_keyframe_sbuffer; }

    private:
        bool create_keyframe_sbuffer();

    private:
        w_ptr<Skeleton> m_OwnerSkeleton;

        uint				m_start_frame_idx;
        uint				m_end_frame_idx;
        uint                m_frame_length;

        double			m_start_time;
        double			m_end_time;
        double			m_time_length;
        float			m_update_time; // 이거 안씀

        uint         	m_frame_per_sec;

        //이중 배열 형태임
        std::vector<tKeyFramesPerBone>          m_key_frames_per_bone;
        s_ptr<StructBuffer>			m_keyframe_sbuffer;
	};
}


