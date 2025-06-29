#include "Editor/Resource/GraphicsShaderEditor.h"

#include <Engine/Manager/PathManager.h>
#include <Engine/Manager/ResourceManager.h>
#include <Engine/Resource/Shader/GraphicsShader.h>
#include <Engine/Global.h>
#include <Engine/Util/Serialize/json.h>

namespace core::editor
{
	namespace name
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

		//constexpr const char* D3D_PRIMITIVE_TOPOLOGY_String[] =
		//{
		//	"D3D_PRIMITIVE_TOPOLOGY_UNDEFINED",
		//	"D3D_PRIMITIVE_TOPOLOGY_POINTLIST",
		//	"D3D_PRIMITIVE_TOPOLOGY_LINELIST",
		//	"D3D_PRIMITIVE_TOPOLOGY_LINESTRIP",
		//	"D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST",
		//	"D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP",
		//	"D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ",
		//	"D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ",
		//	"D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ",
		//	"D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ",
		//};
		//constexpr const size_t D3D_PRIMITIVE_TOPOLOGY_StringSize = sizeof(D3D_PRIMITIVE_TOPOLOGY_String) / sizeof(const char*);
	}


	EditorGraphicsShader::EditorGraphicsShader()
		: EditorWindow(name::EditorGraphicsShader)
		, mDescForEdit()
		, mSemanticEditIdx(-1)
		, mSemanticName{}
		, mInputLayoutDescs{}
		, mDXGIFormatCombo{}
		//, mTopologyCombo{}
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

	void EditorGraphicsShader::init()
	{
		{
			std::vector<tComboItem> Items;
			Items.reserve(name::DXGI_FORMAT_StringSize);
			for (size_t i = 0; i < name::DXGI_FORMAT_StringSize; ++i)
			{
				tComboItem item{};
				item.m_name = name::DXGI_FORMAT_String[i];
				Items.push_back(item);
			}
			mDXGIFormatCombo.set_items(Items);
			mDXGIFormatCombo.set_unique_name("DXGI Format");
		}

		//{
		//	std::vector<tComboItem> Items;
		//	Items.reserve(name::D3D_PRIMITIVE_TOPOLOGY_StringSize);
		//	for (size_t i = 0; i < name::D3D_PRIMITIVE_TOPOLOGY_StringSize; ++i)
		//	{
		//		Items.push_back(tComboItem{ name::D3D_PRIMITIVE_TOPOLOGY_String[i], });
		//	}
		//	mTopologyCombo.set_items(Items);
		//	mTopologyCombo.set_unique_name("Topology Type");
		//}

		{
			std::vector<tComboItem> Items;
			Items.reserve((size_t)eRasterizerState::END);
			for (size_t i = 0; i < (size_t)eRasterizerState::END; ++i)
			{
				tComboItem item{};
				item.m_name = ::core::name::eRasterizerState[i];
				Items.push_back(item);
			}
			mRSTypeCombo.set_items(Items);
			mRSTypeCombo.set_unique_name("Rasterizer Type");
		}

		{
			std::vector<tComboItem> Items;
			Items.reserve((size_t)eDepthStencilState::END);
			for (size_t i = 0; i < (size_t)eDepthStencilState::END; ++i)
			{
				tComboItem item{};
				item.m_name = ::core::name::eDepthStencilState[i];
				Items.push_back(item);
			}
			mDSTypeCombo.set_items(Items);
			mDSTypeCombo.set_unique_name("Depth-Stencil Type");
		}

		{
			std::vector<tComboItem> Items;
			Items.reserve((size_t)eBlendState::END);
			for (size_t i = 0; i < (size_t)eBlendState::END; ++i)
			{
				tComboItem item{};
				item.m_name = ::core::name::eBlendState[i];
				Items.push_back(item);
			}
			mBSTypeCombo.set_items(Items);
			mBSTypeCombo.set_unique_name("Blend State");
		}
		LoadShaderSettingComboBox();

		{
			std::vector<tComboItem> Items;
			Items.reserve((int)eGSStage::END);
			for (size_t i = 0; i < (int)eGSStage::END; ++i)
			{
				tComboItem item{};
				item.m_name = ::core::name::ArrGSPrefix[i];
				Items.push_back(item);
			}
			mStageTypeCombo.set_items(Items);
			mStageTypeCombo.set_unique_name("Shader Type");
		}
	}

	void EditorGraphicsShader::update_UI()
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
				mDXGIFormatCombo.set_current_idx((int)mDescForEdit.Format);
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
			CreateSTDInputLayout(eDimensionType::_2D);
		}

		ImGui::SameLine();

		if (ImGui::Button("Standard 3D Input Layout", ImVec2(0.f, 40.f)))
		{
			CreateSTDInputLayout(eDimensionType::_3D);
		}

		ImGui::Separator();

		//토폴로지 업데이트
		//mTopologyCombo.final_update();

		ImGui::Separator();


		for (size_t i = 0; i < (size_t)eGSStage::END; ++i)
		{
			ImGui::InputText(::core::name::ArrGSPrefix[i].data(), &mStageNames[i]);
		}

		ImGui::Separator();

		//Rasterizer State
		mRSTypeCombo.final_update();

		//Depth Stencil
		mDSTypeCombo.final_update();

		//Blend State
		mBSTypeCombo.final_update();

		
		ImGui::Spacing();
		ImGui::Separator();

		if (ImGui::Button("CreateBuffer Default Shaders", ImVec2(0.f, 50.f)))
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
		using namespace core;

		struct tShaderGroup
		{
			std::array<std::fs::path, (int)eGSStage::END> FileName;
		};
		std::unordered_map<std::fs::path, tShaderGroup> umapGSGroup;

		std::fs::path shaderPath = PathManager::get_inst().GetShaderCSOPath();

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

			for (size_t i = 0; i < (size_t)eGSStage::END; ++i)
			{
				size_t pos = fileName.find(::core::name::ArrGSPrefix[i]);
				if (std::string::npos != pos)
				{
					std::string baseFileName = entry.path().filename().replace_extension(::core::name::path::extension::ShaderSetting).string();
					baseFileName.erase(pos, std::strlen(::core::name::ArrGSPrefix[i].data()));

					umapGSGroup[baseFileName].FileName[i] = fileName;
					break;
				}
			}
		}

		//쉐이더 세팅 파일 경로 확인
		std::vector<tComboItem> new_shader_groups;
		std::fs::path ShaderSettingDir = ResourceManager<GraphicsShader>::get_inst().GetBaseDir();
		if (false == std::fs::exists(ShaderSettingDir))
		{
			std::fs::create_directories(ShaderSettingDir);
		}
		
		//map을 순회 돌아주면서
		for (const auto& iter : umapGSGroup)
		{
			std::fs::path ShaderFilePath = ShaderSettingDir / iter.first.filename().replace_extension(::core::name::path::extension::ShaderSetting);

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
					GraphicsShader DummyGS;
					DummyGS.set_resource_name(iter.first.string());

					const auto& fileNames = iter.second.FileName;

					for (size_t i = 0; i < fileNames.size(); ++i)
					{
						DummyGS.set_shader_name((eGSStage)i, fileNames[i].string());
					}

					
					eResult result = DummyGS.serialize_json(&jVal);
					if (eResult_fail(result))
					{
						ERROR_MESSAGE("json 파일 저장에 실패했습니다.");
						return false;
					}
				}

				//for (int i = 0; i < (int)eGSStage::END; ++i)
				//{
				//	jVal[name::ArrGSPrefix[i]] = iter.second.FileName[i].string();
				//}

				ofs << jVal;
				ofs.close();

				{
					tComboItem item{};
					item.m_name = iter.first.string();
					new_shader_groups.push_back(item);
				}

				mLoadFileCombo.set_items(new_shader_groups);
			}
		}

		//새 쉐이더 그룹이 발견되면 어떤 쉐이더 그룹이 새로 만들었는지 
		if (false == new_shader_groups.empty())
		{
			std::string Message = "New Shader Detected!\n";
			for (size_t i = 0; i < new_shader_groups.size(); ++i)
			{
				Message += new_shader_groups[i].m_name;
				Message += "\n";
			}

			MessageBoxA(nullptr, Message.c_str(), nullptr, MB_OK);
		}

		return true;
	}


	void EditorGraphicsShader::LoadShaderSettingComboBox()
	{
		std::fs::path GSSettingsPath = ResourceManager<GraphicsShader>::get_inst().GetBaseDir();

		if (false == std::fs::exists(GSSettingsPath))
		{
			std::fs::create_directories(GSSettingsPath);
			MessageBoxW(nullptr, L"쉐이더 설정 파일이 없습니다.", nullptr, MB_OK);
			return;
		}

		std::vector<tComboItem> Items;
		for (const auto& entry : std::fs::directory_iterator(GSSettingsPath))
		{
			if (entry.is_directory())
				continue;

			Items.emplace_back(entry.path().filename().string());
		}

		mLoadFileCombo.set_items(Items);
		mLoadFileCombo.set_unique_name("Shader Setting Files");
	}

	void EditorGraphicsShader::DXGISelectCallback(const tComboItem& _item)
	{

	}



	void EditorGraphicsShader::InputElementEditModal()
	{
		using namespace core;
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
				mDXGIFormatCombo.final_update();

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

						const auto& pair = g_stringArchive.insert(mSemanticName);

						mDescForEdit.SemanticName = pair.first->c_str();

						int curSel = mDXGIFormatCombo.get_current_idx();

						if (curSel < 0)
							break;

						if ((DXGI_FORMAT)curSel <= DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE)
							mDescForEdit.Format = (DXGI_FORMAT)curSel;
							//else
							//	mDescForEdit.Format = DXGI_FORMAT_FORCE_UINT;

					} while (false);


					mInputLayoutDescs[mSemanticEditIdx] = mDescForEdit;
					mDXGIFormatCombo.unselect();
					mSemanticEditIdx = -1;
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel", buttonSize))
				{
					mSemanticName.clear();
					mDescForEdit = D3D11_INPUT_ELEMENT_DESC{};
					mDXGIFormatCombo.unselect();

					mSemanticEditIdx = -1;
				}


				ImGui::EndPopup();
			}
		}
	}

	void EditorGraphicsShader::CreateSTDInputLayout(eDimensionType _dimType)
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

		if (_dimType == eDimensionType::_3D)
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

						mStageTypeCombo.unselect();
						mSaveFileName.clear();
						mbSaveModal = false;
						ImGui::CloseCurrentPopup();
					}
				}
				if (ImGui::Button("Cancel"))
				{
					mStageTypeCombo.unselect();
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
				mLoadFileCombo.final_update();

				if (ImGui::Button("Load File"))
				{
					std::string FileName = mLoadFileCombo.get_current_selected().m_name;
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
					mLoadFileCombo.unselect();
					mbLoadModal = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			
		}
	}

	void EditorGraphicsShader::SaveToJson(const std::filesystem::path& _filePath)
	{
		using namespace core;
		GraphicsShader shader{};

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

		//{
		//	int curSel = mTopologyCombo.get_current_idx();
		//	if (curSel >= 0)
		//	{
		//		D3D_PRIMITIVE_TOPOLOGY topology = (D3D_PRIMITIVE_TOPOLOGY)curSel;
		//		shader.set_topology(topology);
		//	}
		//	else
		//	{
		//		NOTIFICATION("Topology가 설정되지 않았습니다.");
		//		return;
		//	}
		//}

		{
			for (size_t i = 0; i < mStageNames.size(); ++i)
			{
				if (0 == i && mStageNames[i].empty())
				{
					NOTIFICATION("필수인 Vertex Shader가 설정되지 않았습니다.");
					return;
				}
				shader.set_shader_name((eGSStage)i, mStageNames[i]);
			}
		}


		{
			int curSel = mRSTypeCombo.get_current_idx();
			if (0 <= curSel && curSel < (int)eRasterizerState::END)
			{
				eRasterizerState Type = (eRasterizerState)curSel;
				shader.set_rasterizer_state(Type);
			}
			else
			{
				NOTIFICATION("Rasterizer Type이 설정되지 않았습니다.");
				return;
			}
		}

		{
			int curSel = mDSTypeCombo.get_current_idx();
			if (0 <= curSel && curSel < (int)eDepthStencilState::END)
			{
				eDepthStencilState Type = (eDepthStencilState)curSel;
				shader.set_depth_stencil_state(Type);
			}
			else
			{
				NOTIFICATION("Depth Stencil Type이 설정되지 않았습니다.");
				return;
			}
		}

		{
			int curSel = mBSTypeCombo.get_current_idx();

			if (0 <= curSel && curSel < (int)eBlendState::END)
			{
				eBlendState Type = (eBlendState)curSel;
				shader.set_blend_state(Type);
			}
			else
			{
				NOTIFICATION("Blend State가 설정되지 않았습니다.");
				return;
			}
		}


		const std::fs::path& baseDir = ResourceManager<GraphicsShader>::get_inst().GetBaseDir();
		
		if (eResult_success(shader.save(baseDir, _filePath)))
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
		using namespace core;
		GraphicsShader shader{};
		shader.set_edit_mode(true);

		const std::fs::path& baseDir = ResourceManager<GraphicsShader>::get_inst().GetBaseDir();
		if (eResult_fail(shader.load(baseDir, _filePath)))
		{
			NOTIFICATION("로드 실패.");
			return;
		}
		
		mSaveFileName = _filePath.string();

		mInputLayoutDescs = shader.GetInputLayoutDescs();
		
		//mTopologyCombo.set_current_idx((int)shader.get_topology());

		for (size_t i = 0; i < mStageNames.size(); ++i)
		{	
			mStageNames[i] = shader.get_shader_name((eGSStage)i);
		}

		mRSTypeCombo.set_current_idx((int)shader.get_rasterizer_state());
		mDSTypeCombo.set_current_idx((int)shader.get_depth_stencil_state());
		mBSTypeCombo.set_current_idx((int)shader.get_blend_state());
	}
}
