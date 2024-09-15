#pragma once
#include "Engine/Common.h"
#include "Engine/GPU/define_GPU.h"
#include "Engine/DefaultShader/Light/Light.hlsli"

namespace ehw {
	class Com_Camera;
	class Renderer;
	class Light_3D;
	class Com_Light;
	class StructBuffer;
	class Texture;

	class SceneRenderAgent
	{
		friend class RenderManager;
	private:
		SceneRenderAgent();
		~SceneRenderAgent();
		void Init();
		void Release();

		void render();

	public:
		void frame_end();

		void SetResolution(UINT _resX, UINT _resY);

		inline Com_Camera* GetMainCamera() { return GetCamera(m_mainCamIndex); }

		void SetMainCamera(Com_Camera* const _pCam);
		Com_Camera* GetCamera(size_t _Idx);

		void Register_camera(Com_Camera* const _pCam);
		void Unregister_camera(Com_Camera* const _pCam);

		void enqueue_render(Renderer* _render);
	private:
		void render_by_mode(Com_Camera* _cam, eRenderingMode _mode);

	private:
		union MeshMaterialKey {
			static_assert(sizeof(size_t) == sizeof(uint64), "size_t가 64bit가 아님. 처리코드 필요");
			struct mm_pair {
				uint32 mesh, material;
			} pair;
			size_t key;	//->이게 m_render_queue의 key
		};

		std::vector<Com_Camera*>			m_cameras;
		size_t								m_mainCamIndex;

		std::shared_ptr<Mesh> m_merge_mesh;	//rectmesh
		std::shared_ptr<Material> m_merge_material;

		std::unordered_map<size_t, tRenderQueue> m_renderer_queues[(int)eRenderingMode::END];
	};
}

