#pragma once
#include <Engine/GPU/GPUBuffer.h>

namespace core
{
    enum class eStructBufferAccessType
    {
        NOT_SET,
        READ_ONLY,      //SRV
        READ_WRITE,     //SRV + UAV(Compute Shader)
        COMPUTE_ONLY,   //UAV
		IMMUTABLE,      //SRV, 생성 후 변경 불가
    };

	class StructBuffer final
		: public GPUBuffer
	{
        CLASS_INFO(StructBuffer, GPUBuffer);
        BASE_RESOURCE(StructBuffer, eResourceType::StructBuffer);
        CLONE_DISABLE(StructBuffer);

    public:
        StructBuffer();
        virtual ~StructBuffer();

        struct tDesc
        {
            tDesc() = delete;
            
            template <typename T>
            tDesc(eStructBufferAccessType _access_type)
                : tDesc((UINT)sizeof(T), _access_type)
            {}

            tDesc(UINT _byte_stride, eStructBufferAccessType _access_type);

            UINT byte_stride = {};
            eStructBufferAccessType access_type = {};
            UINT SRV_slot = UINT_MAX;
            UINT UAV_slot = UINT_MAX;
            UINT capacity = {};

            //생성과 동시에 업로드할 데이터 주소
            const void* p_initial_data = {};
            UINT element_count = {};

            D3D11_BUFFER_DESC buffer_desc = {};
        };

		bool create(const tDesc& _desc);

        bool resize(UINT _new_capacity, const void* _initial_data = nullptr, UINT _elem_count = 0);

        UINT get_capacity() const { return m_capacity; }
        
        UINT get_size() const { return m_size; }

        UINT get_stride() const { return m_byte_stride; }

        //데이터를 버퍼로 전송
        void set_data(const void* _pData, UINT _uCount);

        //데이터를 받아옴
        void get_data(void* _pDest, size_t _uDestByteCapacity);
        template <typename T>
        void get_data(T* _pDest) { get_data(static_cast<void*>(_pDest), sizeof(T)); }

        //데이터를 특정 레지스터에 바인딩. SRV에 바인딩할것인지 UAV에 바인딩할것인지를 지정
        void bind_SRV(eShaderStageFlag_ _pipeline_target = eShaderStageFlag::ALL);

        //Bind buffer with UAV Mode to Compute shader 
        void bind_UAV();

        void unbind();

    private:
        //D3D11_CPU_ACCESS_FLAG
        ComPtr<ID3D11Buffer> create_staging_buffer(UINT _byte_width, bool _enable_read, bool _enable_write);
        ComPtr<ID3D11ShaderResourceView> create_SRV(const ComPtr<ID3D11Buffer>& _buf, UINT _capacity);
        ComPtr<ID3D11UnorderedAccessView> create_UAV(const ComPtr<ID3D11Buffer>& _buf, UINT _capacity);

        virtual eResult save(const std::fs::path& _fullpath) const override;
        virtual eResult load(const std::fs::path& _fullpath) override;

    private:
        eStructBufferAccessType m_access_type = {}; //읽기 전용인지, 읽기/쓰기 모두 가능한지

		UINT                        m_target_SRV_slot = {};
		UINT                        m_target_UAV_slot = {};

        UINT                        m_byte_stride = {};   //구조체 하나 당 바이트 갯수
        UINT                        m_size = {};    //현재 등록한 구조체의 갯수
        UINT                        m_capacity = {}; //현재 확보되어있는 구조체의 갯수

        ComPtr<ID3D11ShaderResourceView> m_SRV = {};

        //RW 형태로 바인딩하고자 할때
        ComPtr<ID3D11UnorderedAccessView> m_UAV = {};

		ComPtr<ID3D11Buffer>    m_readback_buffer = {};
        UINT m_readback_buffer_byte_width = {};

		ComPtr<ID3D11Buffer>    m_upload_buffer = {};
        UINT m_upload_buffer_byte_width = {};

        eBufferViewType             m_current_bound_view = {};

        static inline ComPtr<StructBuffer> s_bound_table_SRV[(int)eShaderStage::END][D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {};
	};


}
