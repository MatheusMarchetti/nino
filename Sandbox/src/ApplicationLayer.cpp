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

	nino::Renderer::DrawTriangle({0.8f, 0.2f, 0.3f, 1.0f}, {0.0f, 0.0f, 5.0f}, { 1.0f, 1.0f, 1.0f });
	nino::Renderer::DrawQuad({1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 15.0f, 5.0f }, { 1.0f, 2.0f, 1.0f }, { 70.0f, 0.0f, 0.0f });

	nino::Renderer::DrawCube({0.2f, 0.5f, 0.6f, 1.0f}, {0.0f, 0.0f, 10.0f}, { 2.0f, 2.0f, 2.0f }, { 0.0f, 0.0f, 0.0f });
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
