
#include "EditorWidget_Tree.h"

namespace editor
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
			SetStrKey("\t" + GetStrKey());
		}
		if (ImGui::TreeNodeEx(GetStrKey().c_str(), flag))
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

	void EditorWidget_Tree::tNode::AddNode(tNode* node)
	{
		node->m_Parent = this;
		m_Childs.push_back(node);
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

	EditorWidget_Tree::tNode* EditorWidget_Tree::AddNode(tNode* parent, const std::string& name, ehw::tDataPtr data, bool isFrame)
	{
		tNode* node = new tNode;
		node->SetStrKey(name);
		node->SetData(data);
		node->SetStem(isFrame);
		node->mTreeWidget = this;

		if (nullptr == parent)
		{
			mRoot = std::unique_ptr<tNode>(node);
		}
		else
		{
			parent->AddNode(node);
		}

		return node;
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