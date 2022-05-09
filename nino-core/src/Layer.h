#pragma once

#include "Core.h"

#include "Events/Event.h"

#include "Timestep.h"

namespace nino
{
	class CORE_API Layer
	{
	public:
		Layer() {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
	};
}