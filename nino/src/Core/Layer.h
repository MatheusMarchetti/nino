#pragma once

#include "Events/Event.h"

#include "Core/Timestep.h"

namespace nino
{
	class Application;

	class Layer
	{
	public:
		Layer() = default;
		Layer(Application* parent) : ParentApplication(parent) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
		virtual void RenderUserInterface() {}

		Application* ParentApplication = nullptr;
	};
}