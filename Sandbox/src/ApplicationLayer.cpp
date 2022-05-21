#include "ApplicationLayer.h"

void ApplicationLayer::OnAttach()
{
	nino::Renderer::ToggleVSync(true);
}

void ApplicationLayer::OnDetach()
{
}

void ApplicationLayer::OnUpdate(nino::Timestep ts)
{
	float color[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	nino::Renderer::SetTargets();
	nino::Renderer::Clear(color);

	nino::Renderer::DrawTriangle();
	nino::Renderer::DrawQuad();
}

void ApplicationLayer::OnEvent(nino::Event& event)
{
	nino::EventManager::Dispatch<PlayerMovedEvent>(BIND_EVENT(ApplicationLayer::OnPlayerMoved));
}

bool ApplicationLayer::OnPlayerMoved(PlayerMovedEvent& event)
{
//	NINO_TRACE(L"Player moved to: ({}, {})", event.GetPosX(), event.GetPosY());

	return true;
}
