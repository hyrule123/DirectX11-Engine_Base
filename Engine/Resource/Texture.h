#pragma once
#include <Engine/Resource/Resource.h>
#include <Engine/Manager/RenderManager.h>

#include <DirectXTex/DirectXTex.h>

namespace core
{
	BASE_RESOURCE(Texture);
	class Texture final : public Resource
	{
		CLASS_INFO(Texture, Resource);
	public:
		Texture();
		virtual ~Texture();

		static void clear_SRV(UINT _startSlot);
		static void clear_ALL();

		//생성 관련 함수
		bool create(UINT _width, UINT _height, DXGI_FORMAT _format, uint _D3D11_BIND_FLAG, bool _bAllowCPURead = false);
		bool create(ComPtr<ID3D11Texture2D> _texture);
		bool create(const D3D11_TEXTURE2D_DESC& _TexDesc);

		//Save / Load
		virtual eResult save(const std::fs::path& _base_directory) const override;
		virtual eResult load(const std::fs::path& _base_directory) override;
		void initialize_resource();


		void bind_buffer_as_SRV(uint _SRVSlot, eShaderStageFlag_ _stageFlag);
		void bind_buffer_as_UAV(uint _UAVSlot = 0u);
		void unbind_buffer_from_GPU_register();


		void set_texture(Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture) { m_texture = _texture; }

		const D3D11_TEXTURE2D_DESC& get_texture2D_desc() const { return m_texture2D_desc; }
		uint get_texture_height() const { return m_texture2D_desc.Height; }
		uint get_texture_width() const { return m_texture2D_desc.Width; }
		uint2 get_texture_size() const { return uint2{ m_texture2D_desc.Width, m_texture2D_desc.Height }; }
		float2 get_texture_size_float() const { 
			return float2{ (float)m_texture2D_desc.Width, (float)m_texture2D_desc.Height }; 
		}

		ComPtr<ID3D11Texture2D> get_texture() { return m_texture; }
		ComPtr<ID3D11DepthStencilView> get_DSV() { return m_DSV; }
		ComPtr<ID3D11RenderTargetView> get_RTV() { return m_RTV; }
		ComPtr<ID3D11UnorderedAccessView> get_UAV() { return m_UAV; }
		ComPtr<ID3D11ShaderResourceView> get_SRV() { return m_SRV; }

	private:
		eResult save_file(const std::filesystem::path& _fullPath) const;
		eResult load_file(const std::filesystem::path& _fullPath);
		bool create_view();

	private:
		D3D11_TEXTURE2D_DESC					m_texture2D_desc;
		ComPtr<ID3D11Texture2D>					m_texture;
		DirectX::ScratchImage							m_image;
		
		//각종 View
		//특정 버퍼와 연결하기 위한 통행증이라고 보면 됨
		ComPtr<ID3D11ShaderResourceView>		m_SRV;
		ComPtr<ID3D11UnorderedAccessView>		m_UAV;
		ComPtr<ID3D11DepthStencilView>			m_DSV;
		ComPtr<ID3D11RenderTargetView>			m_RTV; 

		
		//현재 연결되어있는 버퍼 정보
		eBufferViewType							m_cur_bound_view;
		int										m_cur_bound_register;
		eShaderStageFlag_						m_cur_bound_stage;
	};
}
