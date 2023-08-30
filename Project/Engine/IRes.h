#pragma once
#include "Entity.h"
#include "define_Res.h"
#include "PathMgr.h"
#include "json-cpp/json-forwards.h"

namespace mh
{
	class IRes 
		: public Entity
	{
	public:
		IRes(define::eResourceType _type);

		IRes(const IRes& _other) = default;

		virtual ~IRes();

		//일반적으로 리소스는 별도의 파일이 존재하고, 그것만 로드해주면 되기 때문에
		//Save를 할 일이 없음 -> 딱히 구현 안해도 될경우 재정의 안해도 됨
		//Graphics Shader, Compute Shader 같은 경우에만 Save() 함수를 구현해주면 될듯
		virtual eResult Save(const std::fs::path& _filePath, const std::fs::path& _basePath = L"") { return eResult::Success; }
		virtual eResult Load(const std::fs::path& _filePath, const std::fs::path& _basePath = L"") = 0;

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		define::eResourceType GetResType() { return mType; }

		void SetEngineDefaultRes(bool _bIsDefault) { mbEngineDefaultRes = _bIsDefault; }
		bool IsEngineDefaultRes() const { return mbEngineDefaultRes; }

	protected:
		inline std::fs::path CreateFullPath(const std::fs::path& _filePath, const std::fs::path& _basePath);

	private:
		const define::eResourceType mType;
		bool mbEngineDefaultRes;
	};

	inline std::filesystem::path IRes::CreateFullPath(const std::filesystem::path& _filePath, const std::filesystem::path& _basePath)
	{
		std::filesystem::path fullPath;
		if (_basePath.empty())
		{
			fullPath = PathMgr::GetContentPathRelative(GetResType());
		}
		else
		{
			fullPath = _basePath;
		}
		fullPath /= _filePath;



		return fullPath;
	}
}
