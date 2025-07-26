
#include "Renderer.h"

#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Component/Animator/Animator2D.h>
#include <Engine/Game/Component/Animator/Animator3D.h>

#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>
#include <Engine/Game/GameObject.h>
#include <Engine/Manager/ResourceManager.h>
#include <Engine/Resource/Model3D/Skeleton.h>

#include <Engine/Util/Serialize/JsonSerializer.h>

#include <Engine/Manager/RenderManager.h>


namespace core
{
	Renderer::Renderer()
		: Super(s_component_order)
		, m_mesh(nullptr)
		, m_shared_material(nullptr)
		, m_dynamic_material(nullptr)
		, m_current_material(nullptr)
		, m_material_mode(eMaterialMode::Shared)
		, m_bCullingEnable(true)
	{
	}

	Renderer::~Renderer()
	{
	}

	Renderer::Renderer(const Renderer& _other)
		: Super(_other)
		, m_mesh(_other.m_mesh)
		, m_shared_material()
		, m_dynamic_material(nullptr)
		, m_current_material(nullptr)
		, m_material_mode(_other.m_material_mode)
		, m_bCullingEnable(_other.m_bCullingEnable)
	{
		//Shared Material은 공유 항목이므로 그대로 복사
		m_shared_material = _other.m_shared_material;

		//Dynamic Material은 고유 항목이므로 Clone해서 이쪽도 마찬가지로 고유 항목 생성
		if (_other.m_dynamic_material)
		{
			m_dynamic_material = std::dynamic_pointer_cast<Material>(_other.m_dynamic_material->clone());
		}
	}

	void Renderer::final_update()
	{
		s_ptr<Renderer> ths = std::static_pointer_cast<Renderer>(shared_from_this());
		RenderManager::get_inst().sceneRenderAgent().enqueue_render(ths);
	}

	void Renderer::set_material(const s_ptr<Material>& _mtrl)
	{
		m_shared_material = _mtrl;
		m_current_material = m_shared_material;
	}

	s_ptr<Material> Renderer::set_material_mode(eMaterialMode _mode)
	{
		s_ptr<Material> mtrl = nullptr;
		if (m_shared_material)
		{
			if (eMaterialMode::Shared == _mode)
			{
				m_current_material = m_shared_material;
			}
			else if (eMaterialMode::Dynamic == _mode)
			{
				if (nullptr == m_dynamic_material) {
					m_dynamic_material = std::dynamic_pointer_cast<Material>(m_shared_material->clone());

					ASSERT_DEBUG(m_dynamic_material, "복제 실패");
				}
				m_current_material = m_dynamic_material;
			}
			m_material_mode = _mode;
			mtrl = m_current_material;
		}

		return mtrl;
	}

}
