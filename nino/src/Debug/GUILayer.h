#pragma once

#include "Core/Core.h"

#include "Core/Layer.h"

namespace nino
{
	class GraphicsAPI;
}

namespace nino
{
	class CORE_API GUILayer : public Layer
	{
	public:
		GUILayer(HWND& window, GraphicsAPI& graphicsAPI);
		virtual ~GUILayer() {};

		bool GetMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;

	private:

	};
}