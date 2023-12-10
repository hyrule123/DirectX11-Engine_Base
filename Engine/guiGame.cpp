
#include "PCH_Engine.h"
#include "guiGame.h"

#include "ResourceManager.h"
#include "Texture.h"
#include "GPUManager.h"

namespace editor
{
	guiGame::guiGame()
		: EditorWindow(strKey::GameViewer)
	{
	}

	guiGame::~guiGame()
	{
	}

	void guiGame::UpdateUI()
	{
		std::shared_ptr<ehw::Texture> renderTarget
			= ehw::GPUManager::GetRenderTargetTex();

		std::shared_ptr<ehw::Texture> gameTex
			= std::make_shared<ehw::Texture>();
		gameTex->Create(ehw::GPUManager::GetResolutionX(), ehw::GPUManager::GetResolutionY(), DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		
		//61 번 셰이더 리소스 뷰 null초기화
		ID3D11ShaderResourceView* gameSRV = nullptr;
		auto pContext = ehw::GPUManager::Context();
		pContext->PSSetShaderResources(Register_t_guiGameTexture, 1u, &gameSRV);
		pContext->CopyResource(gameTex->GetTexture().Get()
			, renderTarget->GetTexture().Get());

		gameTex->BindDataSRV(Register_t_guiGameTexture, ehw::eShaderStageFlag::PS);

		ImGuiIO io = ImGui::GetIO();
		ImVec2 panelSize = ImGui::GetWindowSize();
		ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec2 scene_size = ImVec2(io.DisplaySize.x, io.DisplaySize.y);

		ImGui::Image((void*)gameTex->GetSRV().Get(), panelSize
				, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintColor);
	}

}
