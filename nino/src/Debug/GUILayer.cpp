#include "corepch.h"
#include "GUILayer.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "imgui.h"
#include "imgui_internal.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"
#include "Renderer/Drawable/Primitives.h"

#include "Core/Window.h"

#include "Scene/Components.h" //To remove

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace nino
{
	template<typename T, typename Func>
	static void DrawComponent(const std::string& name, Entity entity, Func func)
	{
		ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf;

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeFlags, name.c_str());

			if (open)
			{
				func(component);
				ImGui::TreePop();
			}
		}
	}

	static void DrawVec3Control(const std::string& label, DirectX::XMFLOAT3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void GUILayer::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}

			ImGui::SameLine();

			if (ImGui::Button("Add component"))
				ImGui::OpenPopup("Add component");

			if (ImGui::BeginPopup("Add component"))
			{
				if (ImGui::MenuItem("Transform component"))
				{
					if (!m_SelectionContext.HasComponent<TransformComponent>())
						m_SelectionContext.AddComponent<TransformComponent>();

					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Mesh component"))
				{
					if (!m_SelectionContext.HasComponent<DrawableComponent>())
					{
						m_SelectionContext.AddComponent<DrawableComponent>();
					}

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
				{
					DrawVec3Control("Translation", component.Translation);
					DrawVec3Control("Rotation", component.Rotation);
					DrawVec3Control("Scale", component.Scale, 1.0f);
				});

			DrawComponent<DrawableComponent>("Mesh", entity, [&](auto& component)
				{
					const char* meshTypes[] = { "Cube", "Sphere"};
					static int selectedMesh = 0;
					const char* preview = meshTypes[selectedMesh];

					if (ImGui::BeginCombo("Mesh types", preview, ImGuiComboFlags_NoArrowButton))
					{
						for (int n = 0; n < IM_ARRAYSIZE(meshTypes); n++)
						{
							const bool isSelected = (selectedMesh == n);

							if (ImGui::Selectable(meshTypes[n], isSelected))
							{
								selectedMesh = n;
							}

							if (isSelected)
								ImGui::SetItemDefaultFocus();

						}
						ImGui::EndCombo();
					}

					switch(selectedMesh)
					{
					case 0:		
					{
						m_SelectionContext.GetComponent<DrawableComponent>().Model = nullptr;
						m_SelectionContext.GetComponent<DrawableComponent>().Model = CreateRef<Cube>();
						break;
					}
					case 1:
					{
						m_SelectionContext.GetComponent<DrawableComponent>().Model = nullptr;
						break;
					}
					}
				});
		}
	}

	GUILayer::GUILayer(Window* window)
		: m_Window(window)
	{
		ImGui::CreateContext();
	}

	bool GUILayer::GetMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}

	void GUILayer::OnAttach()
	{
		IMGUI_CHECKVERSION();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplWin32_Init(m_Window->GetWindow());
		ImGui_ImplDX11_Init(GraphicsAPI::GetDevice().Get(), GraphicsAPI::GetContext().Get());
	}

	void GUILayer::Begin()
	{
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX11_NewFrame();
		ImGui::NewFrame();
	}

	void GUILayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)m_Window->GetWidth(), (float)m_Window->GetHeight());

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void GUILayer::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = {};
	}

	void GUILayer::OnDetach()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();

		ImGui::DestroyPlatformWindows();
		ImGui::DestroyContext();
	}

	void GUILayer::TestRender()
	{
//		bool show = true;
//		ImGui::ShowDemoWindow(&show);

		ImGui::Begin("Scene Hierarchy");
			
		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };

				auto& tag = entity.GetComponent<TagComponent>().Tag;

				ImGuiTreeNodeFlags treeFlags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, treeFlags, tag.c_str());
				if (ImGui::IsItemClicked())
				{
					m_SelectionContext = entity;
				}
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create empty entity"))
				m_Context->CreateEntity("Empty entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}
}
