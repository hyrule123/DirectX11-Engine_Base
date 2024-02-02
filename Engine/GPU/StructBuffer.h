#pragma once
#include "GPUBuffer.h"


namespace ehw
{
    enum class eStructBufferType
    {
        READ_ONLY,  //SRV ONLY
        READ_WRITE  //SRV + UAV(Compute Shader)
    };

    //struct tSBufferDesc
    //{
    //    eStructBufferType eSBufferType;

    //    eShaderStageFlag_ TargetStageSRV;

    //    int REGISLOT_t_SRV;
    //    int REGISLOT_u_UAV;

    //    tSBufferDesc()
    //        : eSBufferType()
    //        , TargetStageSRV(eShaderStageFlag::ALL)
    //        , REGISLOT_t_SRV(-1)
    //        , REGISLOT_u_UAV(-1)
    //    {}
    //};

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
        StructBuffer(const Desc& _tDesc);

        StructBuffer(const StructBuffer& _other);
        CLONE(StructBuffer);

        virtual ~StructBuffer();

    public:
        void SetDesc(const Desc& _tDesc);

        //Setter Getter Adder
        void SetPipelineTarget(eShaderStageFlag_ _StageFlag) { m_SbufferDesc.TargetStageSRV = _StageFlag; }
        void AddPipelineTarget(eShaderStageFlag_ _StageFlag) { m_SbufferDesc.TargetStageSRV |= _StageFlag; }

        uint GetCapacity() const { return m_elementCapacity; }

        //글로벌 변수에 있는거 리턴해주면 될듯
        uint GetElemCount() const { return m_elementCount; }

        uint GetStride() const { return m_elementStride; }

        template <typename T>
        HRESULT Create(size_t _ElementCapacity, const void* _pInitialData, size_t _ElemCount);
        //처음 생성할 때 반드시 목표 파이프라인 타겟과 레지스터 번호를 설정해줄 것
        HRESULT Create(size_t _ElementStride, size_t _ElementCapacity, const void* _pInitialData, size_t _ElemCount);

        //데이터를 버퍼로 전송
        void SetData(void* _pData, size_t _uCount);

        //데이터를 받아옴
        void GetData(void* _pDest, size_t _uDestByteCapacity);
        template <typename T>
        void GetData(T* _pDest) { GetData(static_cast<void*>(_pDest), sizeof(T)); }

        //데이터를 특정 레지스터에 바인딩. SRV에 바인딩할것인지 UAV에 바인딩할것인지를 지정
        void BindDataSRV(int _SRVSlot = -1, eShaderStageFlag_ _stageFlag = eShaderStageFlag::NONE);

        //Bind buffer with UAV Mode to Compute shader 
        void BindDataUAV(int _UAVSlot = -1);

        void UnBindData();

    private:
        
        void SetDefaultDesc();

        bool CreateStagingBuffer();
        bool CreateSRV();
        bool CreateUAV();
        


    private:
        Desc                        m_SbufferDesc;

        uint                        m_elementStride;   //구조체 하나 당 바이트 갯수
        uint                        m_elementCount;    //현재 등록한 구조체의 갯수
        uint                        m_elementCapacity; //현재 확보되어있는 구조체의 갯수

        ComPtr<ID3D11ShaderResourceView> m_SRV;

        //RW 형태로 바인딩하고자 할때
        ComPtr<ID3D11UnorderedAccessView> m_UAV;

        ComPtr<ID3D11Buffer>    m_stagingBuffer;

        int                     m_curBoundRegister;
        eBufferViewType         m_curBoundView;
	};

    inline void StructBuffer::SetDesc(const Desc& _tDesc)
    {
        m_SbufferDesc = _tDesc;
        SetDefaultDesc();
    }

    template<typename T>
    inline HRESULT StructBuffer::Create(size_t _ElementCapacity, const void* _pInitialData, size_t _ElemCount)
    {
        return Create(sizeof(T), _ElementCapacity, _pInitialData, _ElemCount);
    }
}
