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
	class StructBuffer 
		: public GPUBuffer
	{
        CLASS_INFO(StructBuffer, GPUBuffer);
        CLONE_ABLE(StructBuffer);
    public:
        struct Desc
        {
            eStructBufferType eSBufferType;

            eShaderStageFlag_ TargetStageSRV;

            int GPU_register_t_SRV;
            int GPU_register_u_UAV;

            Desc()
                : eSBufferType()
                , TargetStageSRV(eShaderStageFlag::ALL)
                , GPU_register_t_SRV(-1)
                , GPU_register_u_UAV(-1)
            {}
        };


    public:
        StructBuffer();

        StructBuffer(const StructBuffer& _other);
        

        virtual ~StructBuffer();

        //desc를 바꾸고자 할 경우 그냥 구조화 버퍼를 지웠다가 다시 생성할것.
        template <typename T>
        eResult init(const Desc& _tDesc, uint _capacity = 0, const void* _pInitialData = nullptr, UINT _ElemCount = 0);

        eResult resize(size_t _ElementCapacity, const void* _pInitialData = nullptr, UINT _dataCount = 0);

        //Setter Getter Adder
        void SetPipelineTarget(eShaderStageFlag_ _StageFlag) { m_desc.TargetStageSRV = _StageFlag; }
        void AddPipelineTarget(eShaderStageFlag_ _StageFlag) { m_desc.TargetStageSRV |= _StageFlag; }

        uint GetCapacity() const { return m_capacity; }

        
        uint get_size() const { return m_size; }

        uint GetStride() const { return m_data_stride; }

        //데이터를 버퍼로 전송
        void set_data(const void* _pData, UINT _uCount);

        //데이터를 받아옴
        void GetData(void* _pDest, size_t _uDestByteCapacity);
        template <typename T>
        void GetData(T* _pDest) { GetData(static_cast<void*>(_pDest), sizeof(T)); }

        //데이터를 특정 레지스터에 바인딩. SRV에 바인딩할것인지 UAV에 바인딩할것인지를 지정
        void bind_buffer_as_SRV(int _SRVSlot = -1, eShaderStageFlag_ _stageFlag = eShaderStageFlag::NONE);

        //Bind buffer with UAV Mode to Compute shader 
        void bind_buffer_as_UAV(int _UAVSlot = -1);

        void unbind_buffer();

    private:
        void SetDesc(const Desc& _tDesc);
        bool CreateStagingBuffer();
        bool CreateSRV();
        bool CreateUAV();

    private:
        Desc                        m_desc;

        UINT                        m_data_stride;   //구조체 하나 당 바이트 갯수
        UINT                        m_size;    //현재 등록한 구조체의 갯수
        UINT                        m_capacity; //현재 확보되어있는 구조체의 갯수

        ComPtr<ID3D11ShaderResourceView> m_SRV;

        //RW 형태로 바인딩하고자 할때
        ComPtr<ID3D11UnorderedAccessView> m_UAV;

        ComPtr<ID3D11Buffer>    m_stagingBuffer;

        int                     m_curBoundRegister;
        eBufferViewType         m_curBoundView;
	};

    template<typename T>
    inline eResult StructBuffer::init(const Desc& _tDesc, UINT _capacity, const void* _pInitialData, UINT _elemCount)
    {
        SetDesc(_tDesc);

        static_assert(sizeof(T) % 16 == 0, "The byte size of the structured buffer must be a multiple of 16.");
        m_data_stride = (UINT)sizeof(T);

        return resize(_capacity, _pInitialData, _elemCount);
    }
}
