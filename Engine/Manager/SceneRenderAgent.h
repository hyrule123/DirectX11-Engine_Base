#pragma once
#include <Engine/Common.h>
#include <Engine/GPU/define_GPU.h>
#include <Engine/DefaultShader/Debug/Debug.hlsli>
#include <Engine/DefaultShader/Light/Light.hlsli>

namespace core {
	class Camera;
	class Renderer;
	class Light3D;
	class Com_Light;
	class StructBuffer;
	class ConstBuffer;
	class Texture;
	class DebugMaterial;

	class SceneRenderAgent
	{
		friend class RenderManager;
	private:
		SceneRenderAgent();
		~SceneRenderAgent();

	public:
		struct tMeshMaterialKey {
			s_ptr<Mesh> mesh;
			s_ptr<Material> material;

			bool operator == (const tMeshMaterialKey& _other) const {
				return (mesh == _other.mesh && material == _other.material);
			}
			struct Hasher {
				size_t operator() (const tMeshMaterialKey& _other) const;
			};
		};
	private:
		void init();
		void release();

		void render();

	public:
		void frame_end();

		void set_resolution(UINT _resX, UINT _resY);

		s_ptr<Camera> get_camera(size_t _idx) {
			if (m_cameras.size() <= _idx) { return nullptr; }
			return m_cameras[_idx].lock();
		}

		void set_main_camera(const s_ptr<Camera>& _pCam);
		s_ptr<Camera> get_main_camera() { return get_camera(m_main_cam_idx); }

		void register_camera(const s_ptr<Camera>& _pCam);
		void unregister_camera(const s_ptr<Camera>& _pCam);

		void enqueue_render(const s_ptr<Renderer>& _render);
		void enqueue_light_3D(eLightType _type, const s_ptr<Light3D>& _light);

		void set_debug_render(bool _is_enable);
		bool is_debug_render_enabled() const { return nullptr != m_debug_material; }

		//World 좌표까지만 넣어두면 이후 Main Camera 기준으로 계산하여 출력한다.
		void enqueue_debug_render(eCollider3D_Shape _shape, const MATRIX& _world, const float3& _RGB) {
			m_debug_draw_data_3D[(int)_shape].push_back({_world, _RGB, });
		}
		void render_debug();
	private:
		void render_by_mode(const s_ptr<Camera>& _cam, eRenderingMode _mode);

	private:
		std::vector<w_ptr<Camera>>			m_cameras;
		size_t								m_main_cam_idx;

		std::unordered_map<tMeshMaterialKey, tRenderQueue, tMeshMaterialKey::Hasher>
			m_renderer_queues[(int)eRenderingMode::END];

		//Light Material

		//Deffered Merge
		s_ptr<Mesh> m_deffered_merge_rectmesh;
		s_ptr<Material> m_deffered_merge_material;

		s_ptr<Mesh> m_debug_meshes_3D[(int)eCollider3D_Shape::END];
		std::vector<tDebugDrawData> m_debug_draw_data_3D[(int)eCollider3D_Shape::END];
		s_ptr<DebugMaterial> m_debug_material;

		std::vector<w_ptr<Light3D>> m_light_3D_instances[(int)eLightType::END];
	};
}

