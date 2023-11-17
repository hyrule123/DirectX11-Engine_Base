#pragma once
#include "guiWindow.h"
#include "guiComboBox.h"
#include <thread>
#include <future>


namespace ehw
{
	class MeshData;
}


namespace gui
{

	class guiFBXConverter :
		public guiWindow
	{
	public:
		guiFBXConverter();
		virtual ~guiFBXConverter();

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

		guiComboBox		mProjMeshDataCombo;
	};
}


