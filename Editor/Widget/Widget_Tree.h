#pragma once
#include "Editor/Base/EditorChild.h"

#include <Engine/Common.h>

#include <functional>

namespace ehw::editor
{
	class EditorWidget_Tree : public EditorChild
	{
	public:
		struct tNode : public EditorEntity
		{
			tNode();
			virtual ~tNode();

			void UpdateUI();

			void set_data(tDataPtr _data) { mData = _data; }

			void SetStem(bool bEnable) { mbStem = bEnable; }

			tNode* AddNode();
			const std::vector<tNode*>& get_childs() { return m_Childs; }

			EditorWidget_Tree* mTreeWidget;
			tDataPtr mData;

			tNode* m_Parent;
			std::vector<tNode*> m_Childs;

			bool mbStem;
			bool mbSelected;
		};

		EditorWidget_Tree();
		virtual ~EditorWidget_Tree();

		virtual void UpdateUI() override;

		tNode* AddNode(tNode* parent, const std::string& name, tDataPtr data, bool stem = false);
		void Clear();
		void SetDummyRoot(bool enable) { mbDummyRootUse = enable; }
		void SetSelectedNode(tNode* node);

		void SetEvent(EditorBase* widget, std::function<void(tDataPtr)> func)
		{
			mEventGUI = widget;
			mEvent = func;
		}


	private:
		std::unique_ptr<tNode> mRoot;

		bool mbDummyRootUse;
		tNode* mSelectedNode;

		EditorBase* mEventGUI;
		std::function<void(tDataPtr data)> mEvent;
	};
}
