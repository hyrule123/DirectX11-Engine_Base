

#include "EditorGraphicsShader.h"

#include "../../Manager/PathManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Resource/Shader/GraphicsShader.h"
#include "../../json-cpp/json.h"


namespace editor
{
	namespace strKey
	{
		constexpr const char* DXGI_FORMAT_String[] =
		{
			"DXGI_FORMAT_UNKNOWN",
			"DXGI_FORMAT_R32G32B32A32_TYPELESS",
			"DXGI_FORMAT_R32G32B32A32_FLOAT",
			"DXGI_FORMAT_R32G32B32A32_UINT",
			"DXGI_FORMAT_R32G32B32A32_SINT",
			"DXGI_FORMAT_R32G32B32_TYPELESS",
			"DXGI_FORMAT_R32G32B32_FLOAT",
			"DXGI_FORMAT_R32G32B32_UINT",
			"DXGI_FORMAT_R32G32B32_SINT",
			"DXGI_FORMAT_R16G16B16A16_TYPELES",
			"DXGI_FORMAT_R16G16B16A16_FLOAT",
			"DXGI_FORMAT_R16G16B16A16_UNORM",
			"DXGI_FORMAT_R16G16B16A16_UINT",
			"DXGI_FORMAT_R16G16B16A16_SNORM",
			"DXGI_FORMAT_R16G16B16A16_SINT",
			"DXGI_FORMAT_R32G32_TYPELESS",
			"DXGI_FORMAT_R32G32_FLOAT",
			"DXGI_FORMAT_R32G32_UINT",
			"DXGI_FORMAT_R32G32_SINT",
			"DXGI_FORMAT_R32G8X24_TYPELESS",
			"DXGI_FORMAT_D32_FLOAT_S8X24_UINT",
			"DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS",
			"DXGI_FORMAT_X32_TYPELESS_G8X24_UINT",
			"DXGI_FORMAT_R10G10B10A2_TYPELESS",
			"DXGI_FORMAT_R10G10B10A2_UNORM",
			"DXGI_FORMAT_R10G10B10A2_UINT",
			"DXGI_FORMAT_R11G11B10_FLOAT",
			"DXGI_FORMAT_R8G8B8A8_TYPELESS",
			"DXGI_FORMAT_R8G8B8A8_UNORM",
			"DXGI_FORMAT_R8G8B8A8_UNORM_SRGB",
			"DXGI_FORMAT_R8G8B8A8_UINT",
			"DXGI_FORMAT_R8G8B8A8_SNORM",
			"DXGI_FORMAT_R8G8B8A8_SINT",
			"DXGI_FORMAT_R16G16_TYPELESS",
			"DXGI_FORMAT_R16G16_FLOAT",
			"DXGI_FORMAT_R16G16_UNORM",
			"DXGI_FORMAT_R16G16_UINT",
			"DXGI_FORMAT_R16G16_SNORM",
			"DXGI_FORMAT_R16G16_SINT",
			"DXGI_FORMAT_R32_TYPELESS",
			"DXGI_FORMAT_D32_FLOAT",
			"DXGI_FORMAT_R32_FLOAT",
			"DXGI_FORMAT_R32_UINT",
			"DXGI_FORMAT_R32_SINT",
			"DXGI_FORMAT_R24G8_TYPELESS",
			"DXGI_FORMAT_D24_UNORM_S8_UINT",
			"DXGI_FORMAT_R24_UNORM_X8_TYPELESS",
			"DXGI_FORMAT_X24_TYPELESS_G8_UINT",
			"DXGI_FORMAT_R8G8_TYPELESS",
			"DXGI_FORMAT_R8G8_UNORM",
			"DXGI_FORMAT_R8G8_UINT",
			"DXGI_FORMAT_R8G8_SNORM",
			"DXGI_FORMAT_R8G8_SINT",
			"DXGI_FORMAT_R16_TYPELESS",
			"DXGI_FORMAT_R16_FLOAT",
			"DXGI_FORMAT_D16_UNORM",
			"DXGI_FORMAT_R16_UNORM",
			"DXGI_FORMAT_R16_UINT",
			"DXGI_FORMAT_R16_SNORM",
			"DXGI_FORMAT_R16_SINT",
			"DXGI_FORMAT_R8_TYPELESS",
			"DXGI_FORMAT_R8_UNORM",
			"DXGI_FORMAT_R8_UINT",
			"DXGI_FORMAT_R8_SNORM",
			"DXGI_FORMAT_R8_SINT",
			"DXGI_FORMAT_A8_UNORM",
			"DXGI_FORMAT_R1_UNORM",
			"DXGI_FORMAT_R9G9B9E5_SHAREDEXP",
			"DXGI_FORMAT_R8G8_B8G8_UNORM",
			"DXGI_FORMAT_G8R8_G8B8_UNORM",
			"DXGI_FORMAT_BC1_TYPELESS",
			"DXGI_FORMAT_BC1_UNORM",
			"DXGI_FORMAT_BC1_UNORM_SRGB",
			"DXGI_FORMAT_BC2_TYPELESS",
			"DXGI_FORMAT_BC2_UNORM",
			"DXGI_FORMAT_BC2_UNORM_SRGB",
			"DXGI_FORMAT_BC3_TYPELESS",
			"DXGI_FORMAT_BC3_UNORM",
			"DXGI_FORMAT_BC3_UNORM_SRGB",
			"DXGI_FORMAT_BC4_TYPELESS",
			"DXGI_FORMAT_BC4_UNORM",
			"DXGI_FORMAT_BC4_SNORM",
			"DXGI_FORMAT_BC5_TYPELESS",
			"DXGI_FORMAT_BC5_UNORM",
			"DXGI_FORMAT_BC5_SNORM",
			"DXGI_FORMAT_B5G6R5_UNORM",
			"DXGI_FORMAT_B5G5R5A1_UNORM",
			"DXGI_FORMAT_B8G8R8A8_UNORM",
			"DXGI_FORMAT_B8G8R8X8_UNORM",
			"DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM",
			"DXGI_FORMAT_B8G8R8A8_TYPELESS",
			"DXGI_FORMAT_B8G8R8A8_UNORM_SRGB",
			"DXGI_FORMAT_B8G8R8X8_TYPELESS",
			"DXGI_FORMAT_B8G8R8X8_UNORM_SRGB",
			"DXGI_FORMAT_BC6H_TYPELESS",
			"DXGI_FORMAT_BC6H_UF16",
			"DXGI_FORMAT_BC6H_SF16",
			"DXGI_FORMAT_BC7_TYPELESS",
			"DXGI_FORMAT_BC7_UNORM",
			"DXGI_FORMAT_BC7_UNORM_SRGB",
			"DXGI_FORMAT_AYUV",
			"DXGI_FORMAT_Y410",
			"DXGI_FORMAT_Y416",
			"DXGI_FORMAT_NV12",
			"DXGI_FORMAT_P010",
			"DXGI_FORMAT_P016",
			"DXGI_FORMAT_420_OPAQUE",
			"DXGI_FORMAT_YUY2",
			"DXGI_FORMAT_Y210",
			"DXGI_FORMAT_Y216",
			"DXGI_FORMAT_NV11",
			"DXGI_FORMAT_AI44",
			"DXGI_FORMAT_IA44",
			"DXGI_FORMAT_P8",
			"DXGI_FORMAT_A8P8",
			"DXGI_FORMAT_B4G4R4A4_UNORM",
			"DXGI_FORMAT_P208",
			"DXGI_FORMAT_V208",
			"DXGI_FORMAT_V408",
			"DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE",
			"DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE",
			//"DXGI_FORMAT_FORCE_UINT",
		};
		constexpr const size_t DXGI_FORMAT_StringSize = sizeof(DXGI_FORMAT_String) / sizeof(const char*);

		constexpr const char* D3D_PRIMITIVE_TOPOLOGY_String[] =
		{
			"D3D_PRIMITIVE_TOPOLOGY_UNDEFINED",
			"D3D_PRIMITIVE_TOPOLOGY_POINTLIST",
			"D3D_PRIMITIVE_TOPOLOGY_LINELIST",
			"D3D_PRIMITIVE_TOPOLOGY_LINESTRIP",
			"D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST",
			"D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP",
			"D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ",
			"D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ",
			"D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ",
			"D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ",
		};
		constexpr const size_t D3D_PRIMITIVE_TOPOLOGY_StringSize = sizeof(D3D_PRIMITIVE_TOPOLOGY_String) / sizeof(const char*);



	}


	EditorGraphicsShader::EditorGraphicsShader()
		: EditorWindow(strKey::EditorGraphicsShader)
		, mDescForEdit()
		, mSemanticEditIdx(-1)
		, mSemanticName{}
		, mInputLayoutDescs{}
		, mDXGIFormatCombo{}
		, mTopologyCombo{}
		, mStageNames{}
		, mRSTypeCombo{}
		, mDSTypeCombo{}
		, mBSTypeCombo{}
		, mbLoadModal()
		, mbSaveModal()
		, mStageTypeCombo()
		, mSaveFileName()
		, mLoadFileCombo()
	{	
	}
	EditorGraphicsShader::~EditorGraphicsShader()
	{
	}

	void EditorGraphicsShader::Init()
	{
		{
			std::vector<EditorWidget_ComboBox::tComboItem> Items;
			Items.reserve(strKey::DXGI_FORMAT_StringSize);
			for (size_t i = 0; i < strKey::DXGI_FORMAT_StringSize; ++i)
			{
				Items.push_back(EditorWidget_ComboBox::tComboItem{ strKey::DXGI_FORMAT_String[i], });
			}
			mDXGIFormatCombo.SetItems(Items);
			mDXGIFormatCombo.SetStrKey("DXGI Format");
		}

		{
			std::vector<EditorWidget_ComboBox::tComboItem> Items;
			Items.reserve(strKey::D3D_PRIMITIVE_TOPOLOGY_StringSize);
			for (size_t i = 0; i < strKey::D3D_PRIMITIVE_TOPOLOGY_StringSize; ++i)
			{
				Items.push_back(EditorWidget_ComboBox::tComboItem{ strKey::D3D_PRIMITIVE_TOPOLOGY_String[i], });
			}
			mTopologyCombo.SetItems(Items);
			mTopologyCombo.SetStrKey("Topology Type");
		}

		{
			std::vector<EditorWidget_ComboBox::tComboItem> Items;
			Items.reserve((size_t)ehw::eRSType::END);
			for (size_t i = 0; i < (size_t)ehw::eRSType::END; ++i)
			{
				Items.push_back(EditorWidget_ComboBox::tComboItem{ ehw::strKey::eRSType[i],});
			}
			mRSTypeCombo.SetItems(Items);
			mRSTypeCombo.SetStrKey("Rasterizer Type");
		}

		{
			std::vector<EditorWidget_ComboBox::tComboItem> Items;
			Items.reserve((size_t)ehw::eDSType::END);
			for (size_t i = 0; i < (size_t)ehw::eDSType::END; ++i)
			{
				Items.push_back(EditorWidget_ComboBox::tComboItem{ ehw::strKey::eDSType[i], });
			}
			mDSTypeCombo.SetItems(Items);
			mDSTypeCombo.SetStrKey("Depth-Stencil Type");
		}

		{
			std::vector<EditorWidget_ComboBox::tComboItem> Items;
			Items.reserve((size_t)ehw::eBSType::END);
			for (size_t i = 0; i < (size_t)ehw::eBSType::END; ++i)
			{
				Items.push_back(EditorWidget_ComboBox::tComboItem{ ehw::strKey::eBSType[i], });
			}
			mBSTypeCombo.SetItems(Items);
			mBSTypeCombo.SetStrKey("Blend State");
		}
		LoadShaderSettingComboBox();

		{
			std::vector<EditorWidget_ComboBox::tComboItem> Items;
			Items.reserve((int)ehw::eGSStage::END);
			for (size_t i = 0; i < (int)ehw::eGSStage::END; ++i)
			{
				Items.push_back(EditorWidget_ComboBox::tComboItem{ ehw::strKey::ArrGSPrefix[i] });
			}
			mStageTypeCombo.SetItems(Items);
			mStageTypeCombo.SetStrKey("Shader Type");
		}
	}

	void EditorGraphicsShader::UpdateUI()
	{
		constexpr const char* cEdit = "Edit##";
		constexpr const char* cDelete = "Delete##";

		for (size_t i = 0; i < mInputLayoutDescs.size(); ++i)
		{
			//Delete 버튼
			std::string Delete = cDelete;
			Delete += std::to_string(i);
			if (ImGui::Button(Delete.c_str()))
			{
				auto mInputElementsIter = mInputLayoutDescs.begin() + i;

				//다음 이터레이터를 받아서 순회 시작
				mInputElementsIter = mInputLayoutDescs.erase(mInputElementsIter);
				break;
			}

			ImGui::SameLine();
			
			//Edit 버튼
			std::string Edit = cEdit;
			Edit += std::to_string(i);
			if (ImGui::Button(Edit.c_str()))
			{
				mSemanticEditIdx = (int)i;

				if (mInputLayoutDescs[i].SemanticName)
					mSemanticName = mInputLayoutDescs[i].SemanticName;
				else
					mSemanticName.clear();
				
				mDescForEdit = mInputLayoutDescs[i];
				mDXGIFormatCombo.SetCurrentIndex((int)mDescForEdit.Format);
			}



			ImGui::SameLine();

			constexpr const char* cSemantic = "Semantic ";
			std::string SemanticNameIdx = cSemantic;
			SemanticNameIdx += std::to_string(i);
			
			if (mInputLayoutDescs[i].SemanticName)
			{
				SemanticNameIdx += ": ";
				SemanticNameIdx += mInputLayoutDescs[i].SemanticName;
			}
				
			ImGui::Text(SemanticNameIdx.c_str());

		}

		InputElementEditModal();

		if(ImGui::Button("Add Input Layout Desc", ImVec2(0.f, 40.f)))
		{
			D3D11_INPUT_ELEMENT_DESC desc{};
			desc.SemanticIndex = (UINT)mInputLayoutDescs.size();
			mInputLayoutDescs.emplace_back(desc);
		}


		if(ImGui::Button("Standard 2D Input Layout", ImVec2(0.f, 40.f)))
		{
			CreateSTDInputLayout(ehw::eDimensionType::_2D);
		}

		ImGui::SameLine();

		if (ImGui::Button("Standard 3D Input Layout", ImVec2(0.f, 40.f)))
		{
			CreateSTDInputLayout(ehw::eDimensionType::_3D);
		}

		ImGui::Separator();

		//토폴로지 업데이트
		mTopologyCombo.InternalUpdate();

		ImGui::Separator();


		for (size_t i = 0; i < (size_t)ehw::eGSStage::END; ++i)
		{
			ImGui::InputText(ehw::strKey::ArrGSPrefix[i], &mStageNames[i]);
		}

		ImGui::Separator();

		//Rasterizer State
		mRSTypeCombo.InternalUpdate();

		//Depth Stencil
		mDSTypeCombo.InternalUpdate();

		//Blend State
		mBSTypeCombo.InternalUpdate();

		
		ImGui::Spacing();
		ImGui::Separator();

		if (ImGui::Button("Create Default Shaders", ImVec2(0.f, 50.f)))
		{
			CreateDefaultShaders();
		}

		ImGui::Separator();

		static const ImVec2 ButtonSize{ 100.f, 50.f };

		ImGui::GetWindowWidth();
		if (ImGui::Button("Save", ButtonSize))
		{
			mbSaveModal = true;
		}
		SaveModal();

		ImGui::SameLine();

		if (ImGui::Button("Load", ButtonSize))
		{
			mbLoadModal = true;
		}
		LoadModal();
	}



	bool EditorGraphicsShader::CreateDefaultShaders()
	{
		struct tShaderGroup
		{
			std::array<std::fs::path, (int)ehw::eGSStage::END> FileName;
		};
		std::unordered_map<std::fs::path, tShaderGroup> umapGSGroup;

		std::fs::path shaderPath = ehw::PathManager::GetShaderCSOPath();

		//쉐이더 바이트코드 경로 확인
		if (false == std::fs::exists(shaderPath))
		{
			std::fs::create_directories(shaderPath);
			MessageBoxW(nullptr, L"쉐이더 파일이 없습니다.", nullptr, MB_OK);
			return false;
		}


		//폴더 순회 돌면서 쉐이더 분류
		for (const auto& entry : std::fs::directory_iterator(shaderPath))
		{
			if (entry.is_directory())
				continue;

			std::string fileName = entry.path().filename().string();

			for (size_t i = 0; i < (size_t)ehw::eGSStage::END; ++i)
			{
				size_t pos = fileName.find(ehw::strKey::ArrGSPrefix[i]);
				if (std::string::npos != pos)
				{
					std::string baseFileName = entry.path().filename().replace_extension(ehw::strKey::path::extension::ShaderSetting).string();
					baseFileName.erase(pos, std::strlen(ehw::strKey::ArrGSPrefix[i]));

					umapGSGroup[baseFileName].FileName[i] = fileName;
					break;
				}
			}
		}



		//쉐이더 세팅 파일 경로 확인
		std::vector<std::string> vecNewShaderGroup;
		std::fs::path ShaderSettingDir = ehw::ResourceManager<ehw::GraphicsShader>::GetBaseDir();
		if (false == std::fs::exists(ShaderSettingDir))
		{
			std::fs::create_directories(ShaderSettingDir);
		}
		
		//map을 순회 돌아주면서
		for (const auto& iter : umapGSGroup)
		{
			std::fs::path ShaderFilePath = ShaderSettingDir / iter.first.filename().replace_extension(ehw::strKey::path::extension::ShaderSetting);

			//파일이 존재하지 않으면 json 파일 초기화 및 생성을 해준다.
			if (false == std::fs::exists(ShaderFilePath))
			{
				std::ofstream ofs(ShaderFilePath);
				if (false == ofs.is_open())
				{
					ERROR_MESSAGE("파일 생성에 실패했습니다.");
					return false;
				}

				Json::Value jVal;
				{
					ehw::GraphicsShader DummyGS;
					DummyGS.SetStrKey(iter.first.string());

					const auto& fileNames = iter.second.FileName;

					for (size_t i = 0; i < fileNames.size(); ++i)
					{
						DummyGS.SetShaderKey((ehw::eGSStage)i, fileNames[i].string());
					}

					if (ehw::eResultFail(DummyGS.SaveJson(&jVal)))
					{
						ERROR_MESSAGE("json 파일 저장에 실패했습니다.");
						return false;
					}
				}

				//for (int i = 0; i < (int)ehw::eGSStage::END; ++i)
				//{
				//	jVal[ehw::strKey::ArrGSPrefix[i]] = iter.second.FileName[i].string();
				//}

				ofs << jVal;
				ofs.close();

				vecNewShaderGroup.push_back(iter.first.string());

				mLoadFileCombo.SetItems(vecNewShaderGroup);
			}
		}

		//새 쉐이더 그룹이 발견되면 어떤 쉐이더 그룹이 새로 만들었는지 
		if (false == vecNewShaderGroup.empty())
		{
			std::string Message = "New Shader Detected!\n";
			for (size_t i = 0; i < vecNewShaderGroup.size(); ++i)
			{
				Message += vecNewShaderGroup[i];
				Message += "\n";
			}

			MessageBoxA(nullptr, Message.c_str(), nullptr, MB_OK);
		}

		return true;
	}


	void EditorGraphicsShader::LoadShaderSettingComboBox()
	{
		std::fs::path GSSettingsPath = ehw::ResourceManager<ehw::GraphicsShader>::GetBaseDir();

		if (false == std::fs::exists(GSSettingsPath))
		{
			std::fs::create_directories(GSSettingsPath);
			MessageBoxW(nullptr, L"쉐이더 설정 파일이 없습니다.", nullptr, MB_OK);
			return;
		}

		std::vector<EditorWidget_ComboBox::tComboItem> Items;
		for (const auto& entry : std::fs::directory_iterator(GSSettingsPath))
		{
			if (entry.is_directory())
				continue;

			Items.emplace_back(entry.path().filename().string());
		}

		mLoadFileCombo.SetItems(Items);
		mLoadFileCombo.SetStrKey("Shader Setting Files");
	}

	void EditorGraphicsShader::DXGISelectCallback(const EditorWidget_ComboBox::tComboItem& _item)
	{

	}



	void EditorGraphicsShader::InputElementEditModal()
	{
		bool bSemanticEditMode = (0 <= mSemanticEditIdx);
		if (bSemanticEditMode)
		{
			ImGui::SetNextWindowSize(ImVec2{ 400.f, 500.f });
			ImGui::OpenPopup("Edit Layout Element");
			if (ImGui::BeginPopupModal("Edit Layout Element"))
			{
				//이름은 별도로 저장(저장 버튼 누를떄 가져옴)
				ImGui::InputText("SemanticName", &mSemanticName);

				//DXGI Format 에딧
				mDXGIFormatCombo.InternalUpdate();

				int ByteOffset = (int)mDescForEdit.AlignedByteOffset;
				if (ImGui::InputInt("AlignedByteOffset", &ByteOffset) && ByteOffset >= 0)
				{
					mDescForEdit.AlignedByteOffset = (UINT)ByteOffset;
				}

				//LPCSTR SemanticName; -> 사용
				//UINT SemanticIndex; -> 미사용(같은 시멘틱 일름으로 여러 개를 만들 경우 사용)
				//DXGI_FORMAT Format; -> 사용
				//UINT InputSlot; -> 미사용
				//UINT AlignedByteOffset; -> 사용
				//D3D11_INPUT_CLASSIFICATION InputSlotClass; -> 인스턴싱에 사용
				//UINT InstanceDataStepRate; -> 미사용

				const static ImVec2 buttonSize(100, 30);
				ImGui::SetCursorPosY(ImGui::GetWindowHeight() - buttonSize.y - 10.f);
				ImGui::Separator();
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonSize.x * 2.f - 30.f);


				if (ImGui::Button("OK", buttonSize))
				{
					do
					{
						if (mSemanticName.empty())
						{
							break;
						}

						const auto& pair = ehw::GraphicsShader::mSemanticNames.insert(mSemanticName);

						mDescForEdit.SemanticName = pair.first->c_str();

						int curSel = mDXGIFormatCombo.GetCurrentIndex();

						if (curSel < 0)
							break;

						if ((DXGI_FORMAT)curSel <= DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE)
							mDescForEdit.Format = (DXGI_FORMAT)curSel;
							//else
							//	mDescForEdit.Format = DXGI_FORMAT_FORCE_UINT;

					} while (false);


					mInputLayoutDescs[mSemanticEditIdx] = mDescForEdit;
					mDXGIFormatCombo.UnSelect();
					mSemanticEditIdx = -1;
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel", buttonSize))
				{
					mSemanticName.clear();
					mDescForEdit = D3D11_INPUT_ELEMENT_DESC{};
					mDXGIFormatCombo.UnSelect();

					mSemanticEditIdx = -1;
				}


				ImGui::EndPopup();
			}
		}
	}

	void EditorGraphicsShader::CreateSTDInputLayout(ehw::eDimensionType _dimType)
	{
		mInputLayoutDescs.clear();

		D3D11_INPUT_ELEMENT_DESC LayoutDesc{};

		LayoutDesc.AlignedByteOffset = 0;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "POSITION";
		LayoutDesc.SemanticIndex = 0;
		mInputLayoutDescs.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

		LayoutDesc.AlignedByteOffset = 16;
		LayoutDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "TEXCOORD";
		LayoutDesc.SemanticIndex = 0;
		mInputLayoutDescs.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

		if (_dimType == ehw::eDimensionType::_3D)
		{
			LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
			LayoutDesc.AlignedByteOffset = 24;
			LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			LayoutDesc.InputSlot = 0;
			LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			LayoutDesc.SemanticName = "TANGENT";
			LayoutDesc.SemanticIndex = 0;
			mInputLayoutDescs.push_back(LayoutDesc);

			LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
			LayoutDesc.AlignedByteOffset = 36;
			LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			LayoutDesc.InputSlot = 0;
			LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			LayoutDesc.SemanticName = "BINORMAL";
			LayoutDesc.SemanticIndex = 0;
			mInputLayoutDescs.push_back(LayoutDesc);

			LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
			LayoutDesc.AlignedByteOffset = 48;
			LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			LayoutDesc.InputSlot = 0;
			LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			LayoutDesc.SemanticName = "NORMAL";
			LayoutDesc.SemanticIndex = 0;
			mInputLayoutDescs.push_back(LayoutDesc);

			LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
			LayoutDesc.AlignedByteOffset = 60;
			LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			LayoutDesc.InputSlot = 0;
			LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			LayoutDesc.SemanticName = "BLENDWEIGHT";
			LayoutDesc.SemanticIndex = 0;
			mInputLayoutDescs.push_back(LayoutDesc);

			LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
			LayoutDesc.AlignedByteOffset = 76;
			LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			LayoutDesc.InputSlot = 0;
			LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			LayoutDesc.SemanticName = "BLENDINDICES";
			LayoutDesc.SemanticIndex = 0;
			mInputLayoutDescs.push_back(LayoutDesc);
		}
	}

	void EditorGraphicsShader::SaveModal()
	{
		if (mbSaveModal)
		{
			ImGui::SetNextWindowSize(ImVec2{ 400.f, 500.f });
			ImGui::OpenPopup("Save Shader");
			if (ImGui::BeginPopupModal("Save Shader"))
			{
				ImGui::InputText("Shader Name", &mSaveFileName);

				if (ImGui::Button("Save File"))
				{
					if (mSaveFileName.empty())
					{
						MessageBoxW(nullptr, L"쉐이더 타입 또는 이름이 제대로 입력되지 않았습니다.", nullptr, MB_OK);

						mSaveFileName.clear();

						mbSaveModal = false;
					}
					else
					{
						std::fs::path FileName = mSaveFileName;

						SaveToJson(FileName);

						mStageTypeCombo.UnSelect();
						mSaveFileName.clear();
						mbSaveModal = false;
						ImGui::CloseCurrentPopup();
					}
				}
				if (ImGui::Button("Cancel"))
				{
					mStageTypeCombo.UnSelect();
					mSaveFileName.clear();
					mbSaveModal = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
	}

	void EditorGraphicsShader::LoadModal()
	{
		if (mbLoadModal)
		{
			ImGui::SetNextWindowSize(ImVec2{ 400.f, 500.f });
			
			ImGui::OpenPopup("Load Shader");
			if (ImGui::BeginPopupModal("Load Shader"))
			{
				mLoadFileCombo.InternalUpdate();

				if (ImGui::Button("Load File"))
				{
					std::string FileName = mLoadFileCombo.GetCurrentSelected().strName;
					if (FileName.empty())
					{
						MessageBoxW(nullptr, L"불러올 쉐이더를 선택하지 않았습니다.", nullptr, MB_OK);
					}
					else
					{
						LoadFromJson(FileName);
					}

					mbLoadModal = false;
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Cancel"))
				{
					mLoadFileCombo.UnSelect();
					mbLoadModal = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			
		}
	}

	void EditorGraphicsShader::SaveToJson(const std::filesystem::path& _filePath)
	{
		ehw::GraphicsShader shader{};

		//이름 등록
		for (size_t i = 0; i < mInputLayoutDescs.size(); ++i)
		{
			if (nullptr == mInputLayoutDescs[i].SemanticName)
			{
				std::wstring errorMsg = std::to_wstring(i);
				errorMsg += L" 번 입력 레이아웃의 Semantic 이름이 존재하지 않습니다.";
				ERROR_MESSAGE_W(errorMsg.c_str());
				return;
			}
		}
		shader.SetInputLayoutDesc(mInputLayoutDescs);

		{
			int curSel = mTopologyCombo.GetCurrentIndex();
			if (curSel >= 0)
			{
				D3D_PRIMITIVE_TOPOLOGY topology = (D3D_PRIMITIVE_TOPOLOGY)curSel;
				shader.SetTopology(topology);
			}
			else
			{
				NOTIFICATION("Topology가 설정되지 않았습니다.");
				return;
			}
		}

		{
			for (size_t i = 0; i < mStageNames.size(); ++i)
			{
				if (0 == i && mStageNames[i].empty())
				{
					NOTIFICATION("필수인 Vertex Shader가 설정되지 않았습니다.");
					return;
				}
				shader.SetShaderKey((ehw::eGSStage)i, mStageNames[i]);
			}
		}


		{
			int curSel = mRSTypeCombo.GetCurrentIndex();
			if (0 <= curSel && curSel < (int)ehw::eRSType::END)
			{
				ehw::eRSType Type = (ehw::eRSType)curSel;
				shader.SetRSState(Type);
			}
			else
			{
				NOTIFICATION("Rasterizer Type이 설정되지 않았습니다.");
				return;
			}
		}

		{
			int curSel = mDSTypeCombo.GetCurrentIndex();
			if (0 <= curSel && curSel < (int)ehw::eDSType::END)
			{
				ehw::eDSType Type = (ehw::eDSType)curSel;
				shader.SetDSState(Type);
			}
			else
			{
				NOTIFICATION("Depth Stencil Type이 설정되지 않았습니다.");
				return;
			}
		}

		{
			int curSel = mBSTypeCombo.GetCurrentIndex();

			if (0 <= curSel && curSel < (int)ehw::eBSType::END)
			{
				ehw::eBSType Type = (ehw::eBSType)curSel;
				shader.SetBSState(Type);
			}
			else
			{
				NOTIFICATION("Blend State가 설정되지 않았습니다.");
				return;
			}
		}

		shader.SetStrKey(_filePath.string());
		if (ehw::eResultSuccess(shader.Save(_filePath)))
		{
			LoadShaderSettingComboBox();
		}
		else
		{
			ERROR_MESSAGE("저장에 실패했습니다.");
		}
	}

	void EditorGraphicsShader::LoadFromJson(const std::filesystem::path& _filePath)
	{
		ehw::GraphicsShader shader{};
		shader.SetEditMode(true);
		if (ehw::eResultFail(shader.Load(_filePath)))
		{
			NOTIFICATION("로드 실패.");
			return;
		}
		
		mSaveFileName = _filePath.string();

		mInputLayoutDescs = shader.mInputLayoutDescs;
		
		mTopologyCombo.SetCurrentIndex((int)shader.GetTopology());

		for (size_t i = 0; i < mStageNames.size(); ++i)
		{	
			mStageNames[i] = shader.GetShaderKey((ehw::eGSStage)i);
		}

		mRSTypeCombo.SetCurrentIndex((int)shader.GetRSState());
		mDSTypeCombo.SetCurrentIndex((int)shader.GetDSState());
		mBSTypeCombo.SetCurrentIndex((int)shader.GetBSState());
	}
}
