#include "ApplicationLayer.h"

#include "imgui.h"

void ApplicationLayer::OnAttach()
{
	nino::Renderer::ToggleVSync(true);
	nino::Renderer::SetViewport(nino::Window::GetWidth(), nino::Window::GetHeight());

	m_TestScene = nino::CreateRef<nino::Scene>();

	//Test scene
#if 1
	auto floor = m_TestScene->CreateEntity("Floor");
	floor.AddComponent<nino::TransformComponent>();
	floor.AddComponent<nino::DrawableComponent>(nino::CreateRef<nino::Cube>());

	auto& floorTranslation = floor.GetComponent<nino::TransformComponent>().Translation;
	auto& floorScale = floor.GetComponent<nino::TransformComponent>().Scale;

	floorTranslation = { 0.0f, -1.1f, 0.7f };
	floorScale = { 5.2f, 0.1f, 2.2f };

	auto Cube = m_TestScene->CreateEntity("Cube");
	Cube.AddComponent<nino::TransformComponent>();
	Cube.AddComponent<nino::DrawableComponent>(nino::CreateRef<nino::Cube>());

	auto& CubeTranslation = Cube.GetComponent<nino::TransformComponent>().Translation;
	auto& CubeScale = Cube.GetComponent<nino::TransformComponent>().Scale;

	CubeTranslation = { 1.9f, -1.0f, 3.4f };
	CubeScale = { 0.4f, 0.4f, 0.4f };

	auto Brick = m_TestScene->CreateEntity("Brick");
	Brick.AddComponent<nino::TransformComponent>();
	Brick.AddComponent<nino::DrawableComponent>(nino::CreateRef<nino::Cube>());

	auto& BrickTranslation = Brick.GetComponent<nino::TransformComponent>().Translation;
	auto& BrickRotation = Brick.GetComponent<nino::TransformComponent>().Rotation;
	auto& BrickScale = Brick.GetComponent<nino::TransformComponent>().Scale;

	BrickTranslation = { 1.6f, -0.7f, 3.4f };
	BrickRotation = { 0.0f, 0.0f, -0.5f };
	BrickScale = { 0.2f, 0.9f, 0.4f };

	auto Building = m_TestScene->CreateEntity("Building");
	Building.AddComponent<nino::TransformComponent>();
	Building.AddComponent<nino::DrawableComponent>(nino::CreateRef<nino::Cube>());

	auto& BuildingTranslation = Building.GetComponent<nino::TransformComponent>().Translation;
	auto& BuildingScale = Building.GetComponent<nino::TransformComponent>().Scale;

	BuildingTranslation = { -1.5f, -0.1f, 2.5f };
	BuildingScale = { 0.6f, 2.1f, 1.0f };

	auto Window1 = m_TestScene->CreateEntity("Window1");
	Window1.AddComponent<nino::TransformComponent>();
	Window1.AddComponent<nino::DrawableComponent>(nino::CreateRef<nino::Cube>());

	auto& Window1Translation = Window1.GetComponent<nino::TransformComponent>().Translation;
	auto& Window1Scale = Window1.GetComponent<nino::TransformComponent>().Scale;

	Window1Translation = { -1.4f, -0.9f, 2.8f };
	Window1Scale = { 0.7f, 0.1f, 0.5f };

	auto Window2 = m_TestScene->CreateEntity("Window2");
	Window2.AddComponent<nino::TransformComponent>();
	Window2.AddComponent<nino::DrawableComponent>(nino::CreateRef<nino::Cube>());

	auto& Window2Translation = Window2.GetComponent<nino::TransformComponent>().Translation;
	auto& Window2Scale = Window2.GetComponent<nino::TransformComponent>().Scale;

	Window2Translation = { -1.4f, -0.8f, 3.8f };
	Window2Scale = { 0.7f, 0.1f, 0.5f };

	auto Window3 = m_TestScene->CreateEntity("Window3");
	Window3.AddComponent<nino::TransformComponent>();
	Window3.AddComponent<nino::DrawableComponent>(nino::CreateRef<nino::Cube>());

	auto& Window3Translation = Window3.GetComponent<nino::TransformComponent>().Translation;
	auto& Window3Scale = Window3.GetComponent<nino::TransformComponent>().Scale;

	Window3Translation = { -1.4f, 0.7f, 2.8f };
	Window3Scale = { 0.7f, 0.1f, 0.5f };

	auto Window4 = m_TestScene->CreateEntity("Window4");
	Window4.AddComponent<nino::TransformComponent>();
	Window4.AddComponent<nino::DrawableComponent>(nino::CreateRef<nino::Cube>());

	auto& Window4Translation = Window4.GetComponent<nino::TransformComponent>().Translation;
	auto& Window4Scale = Window4.GetComponent<nino::TransformComponent>().Scale;

	Window4Translation = { -1.4f, 0.6f, 3.8f };
	Window4Scale = { 0.7f, 0.1f, 0.5f };
#endif

	m_Layer.SetContext(m_TestScene);
}

void ApplicationLayer::OnUpdate(nino::Timestep ts)
{
	float color[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	nino::Renderer::Clear(color);

	m_TestScene->UpdateScene(ts);
}

void ApplicationLayer::OnEvent(nino::Event& event)
{
	nino::EventManager::Dispatch<PlayerMovedEvent>(BIND_EVENT(ApplicationLayer::OnPlayerMoved));
}

void ApplicationLayer::RenderUI()
{
	m_Layer.TestRender();
}

bool ApplicationLayer::OnPlayerMoved(PlayerMovedEvent& event)
{
//	NINO_TRACE(L"Player moved to: ({}, {})", event.GetPosX(), event.GetPosY());

	return true;
}
