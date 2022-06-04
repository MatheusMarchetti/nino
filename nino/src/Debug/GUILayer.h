#pragma once

#include "Core/Core.h"
#include "Core/Layer.h"

#include "Scene/Scene.h" //To remove
#include "Scene/Entity.h" //To remove

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
		GUILayer() = default;
		GUILayer(Window* window);
		virtual ~GUILayer() {};

		bool GetMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void Begin();
		void End();

		void SetContext(const Ref<Scene>& scene);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void TestRender();

	private:
		void DrawComponents(Entity entity);

	private:
		Window* m_Window;

		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}