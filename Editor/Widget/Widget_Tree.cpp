
#include "Widget_Tree.h"

namespace ehw::editor
{
	EditorWidget_Tree::tNode::tNode()
		: EditorEntity("")
		, mTreeWidget(nullptr)
		, mData{}
		, m_Parent(nullptr)
		, mbSelected(false)
		, mbStem(false)
	{
	}
	EditorWidget_Tree::tNode::~tNode()
	{
		for (tNode* child : m_Childs)
		{
			delete child;
			child = nullptr;
		}
	}
	// Node
	void EditorWidget_Tree::tNode::UpdateUI()
	{
		//ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed;

		int flag = 0;

		if (mbStem)
		{
			flag |= ImGuiTreeNodeFlags_Framed;
		}
		if (mbSelected)
		{
			flag |= ImGuiTreeNodeFlags_Selected;
		}
		if (m_Childs.empty())
		{
			flag |= ImGuiTreeNodeFlags_Leaf;
		}
		if (mbStem && m_Childs.empty())
		{
			SetStrKey("\t" + get_keypath());
		}
		if (ImGui::TreeNodeEx(get_keypath().c_str(), flag))
		{
			if (!mbStem && ImGui::IsItemHovered(0) && ImGui::IsMouseClicked(0))
			{
				mTreeWidget->SetSelectedNode(this);
			}

			for (tNode* node : m_Childs)
			{
				node->UpdateUI();
			}

			ImGui::TreePop();
		}
	}

	EditorWidget_Tree::tNode* EditorWidget_Tree::tNode::AddNode()
	{
		m_Childs.push_back(new tNode);
		m_Childs.back()->m_Parent = this;
		return m_Childs.back();
	}



	// Tree
	EditorWidget_Tree::EditorWidget_Tree()
		: EditorChild(strKey::EditorWidget_Tree)
		, mRoot(nullptr)
		, mbDummyRootUse{}
		, mSelectedNode{}
		, mEventGUI{}
		, mEvent{}
	{
	}

	EditorWidget_Tree::~EditorWidget_Tree()
	{
	}


	void EditorWidget_Tree::UpdateUI()
	{
		if (mRoot == nullptr)
		{
			return;
		}

		if (!mbDummyRootUse)
		{
			mRoot->UpdateUI();
		}
		else
		{
			const std::vector<tNode*>& childs = mRoot->GetChilds();
			for (tNode* child : childs)
			{
				child->UpdateUI();
			}
		}
	}

	EditorWidget_Tree::tNode* EditorWidget_Tree::AddNode(tNode* parent, const std::string& name, tDataPtr data, bool isFrame)
	{
		tNode* retNode = nullptr;

		if (nullptr == parent)
		{
			mRoot = std::make_unique<tNode>();
			retNode = mRoot.get();
		}
		else
		{
			retNode = parent->AddNode();
		}

		retNode->SetStrKey(name);
		retNode->SetData(data);
		retNode->SetStem(isFrame);
		retNode->mTreeWidget = this;

		return retNode;
	}

	void EditorWidget_Tree::Clear()
	{
		mRoot.reset();
	}
	void EditorWidget_Tree::SetSelectedNode(tNode* node)
	{
		if (nullptr != mSelectedNode)
		{
			mSelectedNode->mbSelected = false;
		}

		mSelectedNode = node;
		mSelectedNode->mbSelected = true;

		if (mSelectedNode && mEvent)
		{
			mEvent(mSelectedNode->mData);
		}
	}
}
