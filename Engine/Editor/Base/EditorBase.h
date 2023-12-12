#pragma once
#include "EditorEntity.h"

#include "../../Editor/strKey_Editor.h"

namespace editor
{
	class EditorBase 
		: public EditorEntity
	{
	public:
		EditorBase(const std::string_view _strName);
		virtual ~EditorBase();

		virtual ehw::eResult SaveJson(Json::Value* _pJval) override;
		virtual ehw::eResult LoadJson(const Json::Value* _pJval) override;

		void InitRecursive();
		void InternalUpdate();

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


		EditorBase* GetParent() { return m_Parent; }

		template <typename T>
		T* AddChild();
		void AddChild(EditorBase* _pChild);

		void ReserveChildsVector(size_t _size) { m_Childs.reserve(_size); }

		bool GetEnable() { return mbEnable; }
		bool* GetEnablePtr() { return &mbEnable; }
		void SetEnable(bool _bEnable) { mbEnable = _bEnable; }
		void Close() { mbEnable = false; }



		//void SaveRecursive(Json::Value& _Node);
		//void LoadRecursive(Json::Value& _Node);

		void SetNoChild(bool _bNoChild) { mbNoChild = _bNoChild; }
		void SetParent(EditorBase* _Parent) { m_Parent = _Parent; }
		const std::vector<EditorBase*>& GetChilds() { return m_Childs; }
		void RemoveChild(EditorBase* _pChild);


	private:
		EditorBase* m_Parent;
		std::vector<EditorBase*>		m_Childs;		// 자식 EditorBase 목록
		bool						mbNoChild;		// 자식 노드가 들어갈 수 없는 노드로 설정

		bool						mbEnable;
	};

	template<typename T>
	inline T* EditorBase::AddChild()
	{
		static_assert(std::is_base_of_v<EditorBase, T>);

		T* retVal = nullptr;
		if(false == mbNoChild)
		{
			retVal = new T;
			AddChild(retVal);
		}

		return retVal;
	}
}



