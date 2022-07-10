#include "corepch.h"
#include "ImGuiLayer.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "imgui.h"
#include "imgui_internal.h"

#include "Core/Window.h"
#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	ImGuiLayer::ImGuiLayer(Window* window)
		: m_Window(window)
	{
		ImGui::CreateContext();
	}

	void ImGuiLayer::OnAttach()
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

		ImGui_ImplWin32_Init(m_Window->GetHandle());
		ImGui_ImplDX11_Init(GraphicsAPI::GetDevice().Get(), GraphicsAPI::GetContext().Get());
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX11_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
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

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();

		ImGui::DestroyPlatformWindows();
		ImGui::DestroyContext();
	}
}
