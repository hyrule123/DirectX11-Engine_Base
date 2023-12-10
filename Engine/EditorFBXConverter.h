#pragma once
#include "EditorWindow.h"
#include "EditorWidget_ComboBox.h"
#include <thread>
#include <future>


namespace ehw
{
	class MeshData;
}


namespace editor
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

		std::future<ehw::eResult> mFutureConvertResult;

		EditorWidget_ComboBox		mProjMeshDataCombo;
	};
}


