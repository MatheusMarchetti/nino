#pragma once

#include "Core/Core.h"
#include "Core/Layer.h"


namespace nino
{
class Window;

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() = default;
		ImGuiLayer(Window* window);
		virtual ~ImGuiLayer() {};

		void Begin();
		void End();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

	private:
		Window* m_Window;
	};
}