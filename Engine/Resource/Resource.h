#pragma once
#include "Engine/ClassInfo.h"

#include "Engine/Resource/define_Resource.h"

namespace ehw
{
	class Resource 
		: public Entity
		, public std::enable_shared_from_this<Entity>
	{
	public:
		Resource(const std::string_view key);

		Resource(const Resource& _other) = default;

		virtual ~Resource();

		void set_keypath(const std::string_view _key_path) { m_key_path = _key_path; }
		const std::string_view get_keypath() const { return m_key_path; }

		//baseDir = 해당 리소스 모음 폴더까지의 경로. ex: ....Res/Texture
		//strKeyPath = 이후 경로. ex: Player/Player.png -> 리소스를 찾는 키값으로 사용됨
		virtual eResult Save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const;
		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _key_path) = 0;

		void SetEngineDefaultRes(bool _bIsDefault) { mbEngineDefaultRes = _bIsDefault; }
		bool IsEngineDefaultRes() const { return mbEngineDefaultRes; }

		template <typename T>
		inline std::shared_ptr<T> shared_from_this_T();

	private:
		std::string m_key_path;
		bool mbEngineDefaultRes;
	};

	template<typename T>
	inline std::shared_ptr<T> Resource::shared_from_this_T()
	{
		static_assert(std::is_base_of_v<Resource, T>);
		return std::static_pointer_cast<T>(shared_from_this());
	}

}
