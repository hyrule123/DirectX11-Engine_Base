#pragma once
#include <Engine/GPU/GPUBuffer.h>

namespace core
{
    enum class eStructBufferType
    {
        READ_ONLY,  //SRV ONLY
        READ_WRITE  //SRV + UAV(Compute Shader)
    };

    BASE_RESOURCE(StructBuffer);
	class StructBuffer final
		: public GPUBuffer
	{
        CLASS_INFO(StructBuffer, GPUBuffer);
        CLONE_ABLE(StructBuffer);
    public:
        struct tDesc
        {
            eStructBufferType m_buffer_RW_type = eStructBufferType::READ_ONLY;

            eShaderStageFlag_ m_SRV_target_stage = eShaderStageFlag::ALL;

            int32 m_SRV_target_register_idx = -1;
            int32 m_UAV_target_register_idx = -1;
        };

    public:
        StructBuffer();

        StructBuffer(const StructBuffer& _other);
        

        virtual ~StructBuffer();

        //desc를 바꾸고자 할 경우 그냥 구조화 버퍼를 지웠다가 다시 생성할것.
        template <typename T>
        eResult create(const tDesc& _tDesc, uint _capacity = 0, const void* _pInitialData = nullptr, UINT _ElemCount = 0);

        eResult resize(size_t _ElementCapacity, const void* _pInitialData = nullptr, UINT _dataCount = 0);

        //Setter Getter Adder
        void set_pipeline_target(eShaderStageFlag_ _StageFlag) { m_struct_buffer_desc.m_SRV_target_stage = _StageFlag; }
        void add_pipeline_target(eShaderStageFlag_ _StageFlag) { m_struct_buffer_desc.m_SRV_target_stage |= _StageFlag; }

        uint get_capacity() const { return m_capacity; }

        
        uint get_size() const { return m_size; }

        uint get_stride() const { return m_data_stride; }

        //데이터를 버퍼로 전송
        void set_data(const void* _pData, UINT _uCount);

        //데이터를 받아옴
        void get_data(void* _pDest, size_t _uDestByteCapacity);
        template <typename T>
        void get_data(T* _pDest) { get_data(static_cast<void*>(_pDest), sizeof(T)); }

        //데이터를 특정 레지스터에 바인딩. SRV에 바인딩할것인지 UAV에 바인딩할것인지를 지정
        void bind_buffer_as_SRV(int32 _override_SRV_slot = -1, eShaderStageFlag_ _override_stage = eShaderStageFlag::NONE);

        //Bind buffer with UAV Mode to Compute shader 
        void bind_buffer_as_UAV(int32 _override_UAV_slot = -1);

        void unbind_buffer();

    private:
        void set_desc(const tDesc& _tDesc);
        bool create_staging_buffer();
        bool create_SRV();
        bool create_UAV();

    private:
        tDesc                        m_struct_buffer_desc;

        UINT                        m_data_stride;   //구조체 하나 당 바이트 갯수
        UINT                        m_size;    //현재 등록한 구조체의 갯수
        UINT                        m_capacity; //현재 확보되어있는 구조체의 갯수

        ComPtr<ID3D11ShaderResourceView> m_SRV;

        //RW 형태로 바인딩하고자 할때
        ComPtr<ID3D11UnorderedAccessView> m_UAV;

        ComPtr<ID3D11Buffer>    m_staging_buffer;

        int                     m_current_bound_register;
        eBufferViewType         m_current_bound_view;
	};

    template<typename T>
    inline eResult StructBuffer::create(const tDesc& _tDesc, UINT _capacity, const void* _pInitialData, UINT _elemCount)
    {
        set_desc(_tDesc);

        static_assert(sizeof(T) % 16 == 0, "The byte size of the structured buffer must be a multiple of 16.");
        m_data_stride = (UINT)  sizeof(T);

        return resize(_capacity, _pInitialData, _elemCount);
    }
}
