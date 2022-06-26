#pragma once

#include "Core/Core.h"

#include "Renderer/GraphicsAPI/GraphicsCore.h"

namespace nino
{
	class Window;
	class Camera;
}

namespace nino
{
	struct SceneBuffer
	{
		DirectX::XMFLOAT4X4 ViewProjection;
	};

	class Renderer
	{
	public:
		void Init(Window* window);

		static void ToggleVSync(bool vSync) { s_VSync = vSync; }

		static void SetViewport(uint32_t width, uint32_t height, uint32_t topX = 0, uint32_t topY = 0);
		static void Clear(float color[4], float depth = 1.0f);

		static void BeginScene(const Camera& camera);
		static void EndScenes();
		
	private:
		static bool s_VSync;
	};
}
