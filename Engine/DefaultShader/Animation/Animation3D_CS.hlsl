#include <Engine/DefaultShader/Animation/Animation3D_Func.hlsli>

#include <Engine/DefaultShader/ConstBuffer.hlsli>
#include <Engine/DefaultShader/Common_register.hlsli>


//ThreadID.x = Bone Index
//각 본의 위치 정보를 계산하는 compute shader
[numthreads(256, 1, 1)]
void main(uint3 _threadID : SV_DispatchThreadID)
{
	if (g_shared_animation3D_data.BoneCount <= _threadID.x)
	{
		return;
	}

	float4 ZeroRot = float4(0.f, 0.f, 0.f, 1.f);
	matrix matBone = (matrix)0.f;

	int FrameIndex = _threadID.x * g_shared_animation3D_data.FrameLength + g_shared_animation3D_data.CurrentFrame;
	int FrameNextIndex = _threadID.x * g_shared_animation3D_data.FrameLength + g_shared_animation3D_data.NextFrame;

	float4 Scale = lerp(g_FrameTransArray[FrameIndex].Scale, g_FrameTransArray[FrameNextIndex].Scale, g_shared_animation3D_data.FrameRatio);
	float4 Pos = lerp(g_FrameTransArray[FrameIndex].Pos, g_FrameTransArray[FrameNextIndex].Pos, g_shared_animation3D_data.FrameRatio);
	float4 Rot = QuternionLerp(g_FrameTransArray[FrameIndex].RotQuat, g_FrameTransArray[FrameNextIndex].RotQuat, g_shared_animation3D_data.FrameRatio);

	if (g_shared_animation3D_data.bChangingAnim == TRUE)
	{
		uint ChangeFrameIndex = _threadID.x * g_shared_animation3D_data.ChangeFrameLength * g_shared_animation3D_data.ChangeFrameIdx;

		Scale = lerp(Scale, g_ChangeFrameTransArray[ChangeFrameIndex].Scale, g_shared_animation3D_data.ChangeRatio);
		Pos = lerp(Pos, g_ChangeFrameTransArray[ChangeFrameIndex].Pos, g_shared_animation3D_data.ChangeRatio);
		Rot = QuternionLerp(Rot, g_ChangeFrameTransArray[ChangeFrameIndex].RotQuat, g_shared_animation3D_data.ChangeRatio);
	}

	MatrixAffineTransformation(Scale, ZeroRot, Rot, Pos, matBone);

	matrix matOffset = transpose(g_BoneOffsetArray[_threadID.x]);

	//최종 입력 위치 = 자신의 인스턴스 ID * 본의 갯수 + threadID.x
	uint final_index = g_shared_animation3D_data.model_inst_ID * g_shared_animation3D_data.BoneCount + _threadID.x;
	g_FinalBoneMatrixArrayRW[final_index] = mul(matOffset, matBone);
	//g_BoneSocketMatrixArray[_threadID.x].matBone = transpose(matBone);

	//g_BoneSocketMatrixArray[_threadID.x].Scale = Scale.xyz;
	//g_BoneSocketMatrixArray[_threadID.x].Rot = Rot;
	//g_BoneSocketMatrixArray[_threadID.x].Pos = Pos.xyz;

	//g_InstancingBoneMatrixArray[g_shared_animation3D_data.RowIndex * g_shared_animation3D_data.BoneCount + _threadID.x] = mul(matOffset, matBone);
}
