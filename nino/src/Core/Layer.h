#pragma once

#include "Events/Event.h"

#include "Core/Timestep.h"

namespace nino
{
	class Layer
	{
	public:
		Layer() {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
		virtual void RenderUI() {}
	};
}