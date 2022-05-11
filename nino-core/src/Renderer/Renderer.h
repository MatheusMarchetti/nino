#pragma once

#include "Core.h"

#include "Renderer/RendererAPI.h"

namespace nino
{
	class CORE_API Renderer
	{
	public:
		void Create(HWND& window, const uint32_t& width, const uint32_t& height);
		void Resize(const uint32_t& width, const uint32_t& height);
		void ToggleVSync(bool vsync);

	private:
		uint32_t m_Width;
		uint32_t m_Height;
		float m_aspectRatio;
		RendererAPI m_RendererAPI;
	};
}
