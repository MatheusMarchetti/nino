#include "corepch.h"
#include "ImGuiLayer.h"

#include "backends/imgui_impl_win32.h"

#ifdef NINO_API_D3D11
#include "backends/imgui_impl_dx11.h"
#include "Renderer/GraphicsAPI/D3D11/D3D11Backend.h"
#else
#include "backends/imgui_impl_dx12.h"
#endif

#include "imgui.h"
#include "imgui_internal.h"

#include "Core/Window.h"

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

#ifdef NINO_API_D3D11
		ImGui_ImplDX11_Init(D3D11Backend::GetDevice().Get(), D3D11Backend::GetContext().Get());
#else

#endif
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplWin32_NewFrame();

#ifdef NINO_API_D3D11
		ImGui_ImplDX11_NewFrame();
#else

#endif

		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)m_Window->GetWidth(), (float)m_Window->GetHeight());

		ImGui::Render();

#ifdef NINO_API_D3D11
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#else

#endif

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplWin32_Shutdown();

#ifdef NINO_API_D3D11
		ImGui_ImplDX11_Shutdown();
#else

#endif

		ImGui::DestroyPlatformWindows();
		ImGui::DestroyContext();
	}
}
