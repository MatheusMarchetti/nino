#include "corepch.h"
#include "GraphicsAPI.h"

#include "Core/Window.h"

namespace nino
{
	const Microsoft::WRL::ComPtr<IDXGISwapChain4> GraphicsAPI::GetSwapChain(const Window* window)
	{
		return s_RenderTargets[window->GetHandle()];
	}
}