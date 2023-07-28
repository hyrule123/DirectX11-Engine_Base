
#include "EnginePCH.h"

#include "Mesh.h"
#include "RenderMgr.h"
#include "GraphicDevice_DX11.h"

namespace mh
{
	Mesh::Mesh()
		: IRes(eResourceType::Mesh)
		, mVBDesc{}
		, mVertexByteStride()
		, mVertexCount()
		, mIBDesc{}
		, mIndexCount(0)
	{

	}

	Mesh::~Mesh()
	{

	}

	HRESULT Mesh::Load(const std::filesystem::path& _path)
	{
		return E_NOTIMPL;
	}

	bool Mesh::CreateVertexBuffer(void* _data, size_t _dataStride, size_t _count)
	{
		mVertexByteStride = (UINT)_dataStride;
		mVertexCount = (UINT)_count;
		// 버텍스 버퍼
		mVBDesc.ByteWidth = mVertexByteStride * mVertexCount;
		mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		mVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mVBDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = _data;

		bool Result = GPU::GetDevice()->CreateBuffer(&mVBDesc, &subData, mVertexBuffer.GetAddressOf());

		if(false == Result)
		{
			mVertexBuffer = nullptr;
			mVBDesc = {};
			mVertexByteStride = 0u;
			mVertexCount = 0u;
		}
			
		return Result;
	}

	bool Mesh::CreateIndexBuffer(void* _data, size_t _count)
	{
		mIndexCount = (UINT)_count;
		mIBDesc.ByteWidth = (UINT)(sizeof(UINT) * _count);
		mIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		mIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mIBDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = _data;

		bool bResult = GPU::GetDevice()->CreateBuffer(&mIBDesc, &subData, mIndexBuffer.GetAddressOf());

		if (false == bResult)
		{
			mIndexBuffer = nullptr;
			mIndexCount = 0u;
			mIBDesc = {};
		}

		return bResult;
	}

	void Mesh::BindBuffer() const
	{
		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		UINT offset = 0;

		GPU::GetDevice()->BindVertexBuffer(0, 1, mVertexBuffer.GetAddressOf(), &mVertexByteStride, &offset);
		GPU::GetDevice()->BindIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void Mesh::Render() const
	{
		GPU::GetDevice()->DrawIndexed(mIndexCount, 0, 0);
	}
	
	void Mesh::RenderInstanced(UINT _count) const
	{
		GPU::GetDevice()->DrawIndexedInstanced(mIndexCount, _count, 0, 0, 0);
	}
}
