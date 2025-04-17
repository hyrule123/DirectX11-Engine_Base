#pragma once
#include "Engine/Common.h"
#include "Engine/GPU/define_GPU.h"
#include "Engine/DefaultShader/Debug/Debug.hlsli"
#include "Engine/DefaultShader/Light/Light.hlsli"

namespace core {
	class Com_Camera;
	class Renderer;
	class Light_3D;
	class Com_Light;
	class StructBuffer;
	class ConstBuffer;
	class Texture;
	class DebugMaterial;

	class SceneRenderAgent
	{
		friend class RenderManager;
	public:
		struct MeshMaterialKey {
			std::shared_ptr<Mesh> mesh;
			std::shared_ptr<Material> material;

			bool operator == (const MeshMaterialKey& _other) const {
				return (mesh == _other.mesh && material == _other.material);
			}
			struct Hasher {
				size_t operator() (const MeshMaterialKey& _other) const;
			};
		};
	private:
		SceneRenderAgent();
		~SceneRenderAgent();
		void init();
		void release();

		void render();

	public:
		void frame_end();

		void SetResolution(UINT _resX, UINT _resY);

		Com_Camera* GetCamera(size_t _Idx);

		void SetMainCamera(Com_Camera* const _pCam);
		Com_Camera* GetMainCamera() { return GetCamera(m_mainCamIndex); }

		void Register_camera(Com_Camera* const _pCam);
		void Unregister_camera(Com_Camera* const _pCam);

		void enqueue_render(Renderer* _render);
		void enqueue_light_3D(eLightType _type, Light_3D* _light) {
			if (_light) {
				m_light_3D_instances[(int)_type].push_back(_light);
			}
		}

		void set_debug_render(bool _is_enable);
		bool get_debug_render() const { return nullptr != m_debug_material; }

		//World 좌표까지만 넣어두면 이후 Main Camera 기준으로 계산하여 출력한다.
		void enqueue_debug_render(eCollider3D_Shape _shape, const MATRIX& _world, const float3& _RGB) {
			m_debug_draw_data_3D[(int)_shape].push_back({_world, _RGB, });
		}
		void render_debug();
	private:
		void render_by_mode(Com_Camera* _cam, eRenderingMode _mode);

	private:
		std::vector<Com_Camera*>			m_cameras;
		size_t								m_mainCamIndex;

		std::unordered_map<MeshMaterialKey, tRenderQueue, MeshMaterialKey::Hasher>
			m_renderer_queues[(int)eRenderingMode::END];

		//Light Material

		//Deffered Merge
		std::shared_ptr<Mesh> m_deffered_merge_rectmesh;
		std::shared_ptr<Material> m_deffered_merge_material;

		std::shared_ptr<Mesh> m_debug_meshes_3D[(int)eCollider3D_Shape::END];
		std::vector<tDebugDrawData> m_debug_draw_data_3D[(int)eCollider3D_Shape::END];
		std::shared_ptr<DebugMaterial> m_debug_material;

		std::vector<Light_3D*> m_light_3D_instances[(int)eLightType::END];
	};
}

