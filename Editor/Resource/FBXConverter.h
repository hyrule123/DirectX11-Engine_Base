#pragma once
#include "Editor/Base/EditorWindow.h"

#include "Editor/Widget/Widget_ComboBox.h"

#include <thread>
#include <future>

namespace ehw
{
	class Model3D;
}


namespace ehw::editor
{
	class EditorFBXConverter :
		public EditorWindow
	{
	public:
		EditorFBXConverter();
		virtual ~EditorFBXConverter();

		virtual void Init() override;
		virtual void UpdateUI() override;

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

		EditorWidget_ComboBox		mProjMeshDataCombo;
	};
}


