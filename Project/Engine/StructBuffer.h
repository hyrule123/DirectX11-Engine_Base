#pragma once

#include "GPUBuffer.h"

namespace mh
{
    enum class eStructBufferType
    {
        READ_ONLY,  //SRV ONLY
        READ_WRITE  //SRV + UAV(Compute Shader)
    };

    //struct tSBufferDesc
    //{
    //    eStructBufferType eSBufferType;

    //    define::eShaderStageFlag_ TargetStageSRV;

    //    int REGISLOT_t_SRV;
    //    int REGISLOT_u_UAV;

    //    tSBufferDesc()
    //        : eSBufferType()
    //        , TargetStageSRV(define::eShaderStageFlag::ALL)
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

            define::eShaderStageFlag_ TargetStageSRV;

            int REGISLOT_t_SRV;
            int REGISLOT_u_UAV;

            Desc()
                : eSBufferType()
                , TargetStageSRV(define::eShaderStageFlag::ALL)
                , REGISLOT_t_SRV(-1)
                , REGISLOT_u_UAV(-1)
            {}
        };


    public:

        StructBuffer(const Desc& _tDesc);

        StructBuffer(const StructBuffer& _other);
        CLONE(StructBuffer);

        virtual ~StructBuffer();

    public:
        //Setter Getter Adder
        void SetPipelineTarget(define::eShaderStageFlag_ _StageFlag) { mSBufferDesc.TargetStageSRV = _StageFlag; }
        void AddPipelineTarget(define::eShaderStageFlag_ _StageFlag) { mSBufferDesc.TargetStageSRV |= _StageFlag; }

        uint GetCapacity() const { return mElementCapacity; }

        //글로벌 변수에 있는거 리턴해주면 될듯
        uint GetElemCount() const { return mElementCount; }

        uint GetStride() const { return mElementStride; }

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
        void BindDataSRV(int _SRVSlot = -1, define::eShaderStageFlag_ _stageFlag = define::eShaderStageFlag::NONE);

        //Bind buffer with UAV Mode to Compute shader 
        void BindDataUAV(int _UAVSlot = -1);

        void UnBindData();

    private:
        void SetDesc(const Desc& _tDesc);
        void SetDefaultDesc();

        bool CreateStagingBuffer();
        bool CreateSRV();
        bool CreateUAV();
        


    private:
        Desc                        mSBufferDesc;

        uint                        mElementStride;   //구조체 하나 당 바이트 갯수
        uint                        mElementCount;    //현재 등록한 구조체의 갯수
        uint                        mElementCapacity; //현재 확보되어있는 구조체의 갯수

        ComPtr<ID3D11ShaderResourceView> mSRV;

        //RW 형태로 바인딩하고자 할때
        ComPtr<ID3D11UnorderedAccessView> mUAV;

        ComPtr<ID3D11Buffer>    mStagingBuffer;

        int                     mCurBoundRegister;
        define::eBufferViewType         mCurBoundView;
	};

    inline void StructBuffer::SetDesc(const Desc& _tDesc)
    {
        mSBufferDesc = _tDesc;
        SetDefaultDesc();
    }

    template<typename T>
    inline HRESULT StructBuffer::Create(size_t _ElementCapacity, const void* _pInitialData, size_t _ElemCount)
    {
        return Create(sizeof(T), _ElementCapacity, _pInitialData, _ElemCount);
    }
}
