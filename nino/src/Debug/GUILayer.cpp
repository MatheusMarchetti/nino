#include "corepch.h"
#include "GUILayer.h"

#include "nino/vendor/imgui/backends/imgui_impl_dx11.h"
#include "nino/vendor/imgui/backends/imgui_impl_win32.h"
#include "nino/vendor/imgui/imgui.h"

#include "Renderer/GraphicsAPI.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace nino
{
	GUILayer::GUILayer(HWND& window, GraphicsAPI& graphicsAPI)
	{
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(graphicsAPI.GetDevice().Get(), graphicsAPI.GetContext().Get());
	}

	bool GUILayer::GetMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}

	void GUILayer::OnAttach()
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();
	}

	void GUILayer::OnDetach()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();

		ImGui::DestroyContext();
		ImGui::DestroyPlatformWindows();
	}

	void GUILayer::OnUpdate(Timestep ts)
	{
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX11_NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	void GUILayer::OnEvent(Event& event)
	{
	}
}
