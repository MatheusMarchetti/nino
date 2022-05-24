#include "corepch.h"
#include "GUILayer.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "imgui.h"

#include "Renderer/GraphicsAPI.h"
#include "Core/Window.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace nino
{
	GUILayer::GUILayer(Window* window, GraphicsAPI* graphicsAPI)
		: m_Window(window), m_GraphicsAPI(graphicsAPI)
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
		ImGui_ImplDX11_Init(m_GraphicsAPI->GetDevice().Get(), m_GraphicsAPI->GetContext().Get());
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

	void GUILayer::OnDetach()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();

		ImGui::DestroyPlatformWindows();
		ImGui::DestroyContext();
	}
}
