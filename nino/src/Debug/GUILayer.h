#pragma once

#include "Core/Layer.h"

namespace nino
{
	class Window;
	class GraphicsAPI;
}

namespace nino
{
	class GUILayer : public Layer
	{
	public:
		GUILayer(Window* window, GraphicsAPI* graphicsAPI);
		virtual ~GUILayer() {};

		bool GetMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void Begin();
		void End();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

	private:
		Window* m_Window;
		GraphicsAPI* m_GraphicsAPI;
	};
}