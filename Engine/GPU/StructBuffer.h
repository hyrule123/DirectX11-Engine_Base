#pragma once
#include "Engine/GPU/GPUBuffer.h"

namespace ehw
{
    enum class eStructBufferType
    {
        READ_ONLY,  //SRV ONLY
        READ_WRITE  //SRV + UAV(Compute Shader)
    };

	class StructBuffer 
		: public GPUBuffer
	{
    public:
        struct Desc
        {
            eStructBufferType eSBufferType;

            eShaderStageFlag_ TargetStageSRV;

            int REGISLOT_t_SRV;
            int REGISLOT_u_UAV;

            Desc()
                : eSBufferType()
                , TargetStageSRV(eShaderStageFlag::ALL)
                , REGISLOT_t_SRV(-1)
                , REGISLOT_u_UAV(-1)
            {}
        };


    public:
        StructBuffer();

        StructBuffer(const StructBuffer& _other);
        CLONE_ABLE(StructBuffer);

        virtual ~StructBuffer();

        //desc를 바꾸고자 할 경우 그냥 구조화 버퍼를 지웠다가 다시 생성할것.
        template <typename T>
        eResult Init(const Desc& _tDesc, uint _capacity = 0, const void* _pInitialData = nullptr, size_t _ElemCount = 0);

        eResult Resize(size_t _ElementCapacity, const void* _pInitialData = nullptr, size_t _dataCount = 0);

        //Setter Getter Adder
        void SetPipelineTarget(eShaderStageFlag_ _StageFlag) { m_desc.TargetStageSRV = _StageFlag; }
        void AddPipelineTarget(eShaderStageFlag_ _StageFlag) { m_desc.TargetStageSRV |= _StageFlag; }

        uint GetCapacity() const { return m_capacity; }

        //글로벌 변수에 있는거 리턴해주면 될듯
        uint GetElemCount() const { return m_elementCount; }

        uint GetStride() const { return m_elementStride; }

        //데이터를 버퍼로 전송
        void SetData(const void* _pData, size_t _uCount);

        //데이터를 받아옴
        void GetData(void* _pDest, size_t _uDestByteCapacity);
        template <typename T>
        void GetData(T* _pDest) { GetData(static_cast<void*>(_pDest), sizeof(T)); }

        //데이터를 특정 레지스터에 바인딩. SRV에 바인딩할것인지 UAV에 바인딩할것인지를 지정
        void BindDataSRV(int _SRVSlot = -1, eShaderStageFlag_ _stageFlag = eShaderStageFlag::NONE);

        //Bind buffer with UAV Mode to Compute shader 
        void BindDataUAV(int _UAVSlot = -1);

        void UnbindData();

    private:
        void SetDesc(const Desc& _tDesc);
        bool CreateStagingBuffer();
        bool CreateSRV();
        bool CreateUAV();

    private:
        Desc                        m_desc;

        uint                        m_elementStride;   //구조체 하나 당 바이트 갯수
        uint                        m_elementCount;    //현재 등록한 구조체의 갯수
        uint                        m_capacity; //현재 확보되어있는 구조체의 갯수

        ComPtr<ID3D11ShaderResourceView> m_SRV;

        //RW 형태로 바인딩하고자 할때
        ComPtr<ID3D11UnorderedAccessView> m_UAV;

        ComPtr<ID3D11Buffer>    m_stagingBuffer;

        int                     m_curBoundRegister;
        eBufferViewType         m_curBoundView;
	};

    template<typename T>
    inline eResult StructBuffer::Init(const Desc& _tDesc, uint _capacity, const void* _pInitialData, size_t _elemCount)
    {
        SetDesc(_tDesc);

        static_assert(sizeof(T) % 16 == 0, "The byte size of the structured buffer must be a multiple of 16.");
        m_elementStride = (uint)sizeof(T);

        return Resize(_capacity, _pInitialData, _elemCount);
    }
}
