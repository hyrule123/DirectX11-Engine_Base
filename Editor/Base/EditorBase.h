#pragma once
#include "Editor/Base/EditorEntity.h"
#include "strKey_Editor.h"

#include <Engine/Util/Serialize/Serializable.h>

namespace ehw::editor
{
	class EditorBase 
		: public EditorEntity
		, public Serializable_Json
	{
	public:
		EditorBase(const std::string_view _strName);
		virtual ~EditorBase();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		void InitRecursive();
		void FinalUpdate();

		virtual void Init() {}
		virtual void Update() {}

		//재정의해서 각자 guiBase에 맞는 begin함수를 호출. bool 값이 반환되며, true가 반환되었을 때에만 endUI()가 호출됨.
		//그러므로 무조건 endUI()를 호출하고 싶을 경우에는 true를 고정으로 반환시켜줄 것 
		//bool값을 반환 여부에 따라 endUI() 호출 여부가 달라지는 IMGUI 함수(ex.ComboBox)의 경우에는
		//return 값으로 해당 함수를 호출하면 된다.
		virtual bool BeginUI() = 0;

		//여기에는 클래스에서 특별하게 호출해야할 레이아웃이 있을 경우 재정의해서 사용하면 됨.
		virtual void UpdateUI() {}

		//재정의해서 각자 guiBase에 맞는 end함수를 호출
		virtual void EndUI() = 0;


		const std::weak_ptr<EditorBase>& GetParent() { return m_parent; }

		template <typename T>
		std::shared_ptr<T> AddChild();

		std::shared_ptr<EditorBase> AddChild(const std::shared_ptr<EditorBase>& _pChild);
		void ClearChild();

		void ReserveChildsVector(size_t _size) { m_childs.reserve(_size); }

		bool GetEnable() { return m_bEnable; }
		bool* GetEnablePtr() { return &m_bEnable; }
		void SetEnable(bool _bEnable) { m_bEnable = _bEnable; }
		void Close() { m_bEnable = false; }



		//void SaveRecursive(Json::Value& _Node);
		//void LoadRecursive(Json::Value& _Node);

		void SetNoChild(bool _bNoChild) { m_bNoChild = _bNoChild; }
		void SetParent(const std::shared_ptr<EditorBase>& _Parent) { m_parent = _Parent; }
		const std::vector<std::shared_ptr<EditorBase>>& GetChilds() { return m_childs; }
		void RemoveChild(const std::shared_ptr<EditorBase>& _pChild);


	private:
		std::weak_ptr<EditorBase> m_parent;
		std::vector<std::shared_ptr<EditorBase>>		m_childs;		// 자식 EditorBase 목록
		bool						m_bNoChild;		// 자식 노드가 들어갈 수 없는 노드로 설정

		bool						m_bEnable;
	};

	template<typename T>
	inline std::shared_ptr<T> EditorBase::AddChild()
	{
		static_assert(std::is_base_of_v<EditorBase, T>);

		std::shared_ptr<T> retVal = nullptr;
		if(false == m_bNoChild)
		{
			retVal = std::make_shared<T>();
			const std::shared_ptr<EditorBase>& result = AddChild(std::static_pointer_cast<EditorBase>(retVal));
			
			if (nullptr == result)
			{
				retVal = nullptr;
			}
		}

		return retVal;
	}
}



