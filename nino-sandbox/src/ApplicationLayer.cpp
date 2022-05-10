#include "ApplicationLayer.h"

#include <thread>

void ApplicationLayer::OnAttach()
{
}

void ApplicationLayer::OnDetach()
{
}

void ApplicationLayer::OnUpdate(nino::Timestep ts)
{
	std::shared_ptr<PlayerMovedEvent> playerMoved = std::make_shared<PlayerMovedEvent>(PlayerX, PlayerY);
	nino::EventManager::QueueEvents(playerMoved);

//	PlayerX += 1.0f * ts;
//	PlayerY -= 2.0f * ts;
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
