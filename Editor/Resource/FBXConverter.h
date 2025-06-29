#pragma once
#include "Editor/Base/EditorWindow.h"

#include "Editor/Widget/Widget_ComboBox.h"

#include <thread>
#include <future>

namespace core
{
	class Model3D;
}


namespace core::editor
{
	class EditorFBXConverter :
		public EditorWindow
	{
	public:
		EditorFBXConverter();
		virtual ~EditorFBXConverter();

		virtual void init() override;
		virtual void update_UI() override;

	private:
		bool CheckThread();
		void ChooseFBXButton();
		void ConvertFBXButton();
		void AddAnimationFromSameModeling();

		void LoadProjMeshDataCombo();

	private:
		std::string		mFBXPath;
		std::string		mOutputDirName;
		bool			mbStatic;

		std::future<eResult> mFutureConvertResult;

		EditorWidget_ComboBox		m_model3D_combobox;
	};
}


