#include "ApplicationLayer.h"

#include "imgui.h"

using namespace nino;

void ApplicationLayer::OnAttach()
{
	auto nativeWindow = ParentApplication->GetNativeWindow();

	FramebufferDescriptor mainFrameDesc;
	mainFrameDesc.Name = nativeWindow->GetDescriptor().WindowName;
	mainFrameDesc.ColorResource = CreateRef<Texture>(nativeWindow);
	mainFrameDesc.DepthStencilResource = CreateRef<Texture>(TextureType::Texture2D, TextureUsage::DepthBinding, nativeWindow->GetWidth(), nativeWindow->GetHeight());
	mainFrameDesc.ViewportSpecification = { 0, 0, (float)nativeWindow->GetWidth(), (float)nativeWindow->GetHeight() };

	m_MainFramebuffer = CreateRef<Framebuffer>(mainFrameDesc);

	m_MainCamera.ChangeCameraType(CameraType::Perspective);
	m_MainCamera.SetViewportSize((float)nativeWindow->GetWidth(), (float)nativeWindow->GetHeight());
	m_MainCamera.SetFieldOfView(45.0f);
}

void ApplicationLayer::OnUpdate(Timestep ts)
{
	Color clearColor = { 0.4f, 0.5f, 0.8f, 1.0f };

	m_MainFramebuffer->Clear(clearColor, 1.0f);

	RenderManager::DrawGrid(5.0f, 5.0f);

	RenderManager::BeginScene(m_MainCamera);
	{
		Vector3 position = { 0.0f, 1.0f, 0.0f };
		Vector3 rotation = { angle, angle, angle };
		Vector3 scale = { 0.5f, 0.5f, 0.5f };
		Vector4 color = { 0.2f * (sinf(angle) + 1.0f)/2.0f, 0.5f * (sinf(angle) + 1.0f) / 2.0f, 0.8f * (sinf(angle) + 1.0f) / 2.0f, 1.0f };

		PrimitiveDescriptor cube1 = {};
		cube1.Type = PrimitiveType::Cube;
		cube1.Position = position;
		cube1.Rotation = rotation;
		cube1.Scale = scale;
		cube1.Color = color;

		Material testMaterial("Assets/Textures/environment.hdr", "Assets/Shaders/BasicPBR.hlsl");

		RenderManager::DrawPrimitive(cube1);
	}
	RenderManager::EndScene({ m_MainFramebuffer });

	angle += ts;

	int x, y, offset;

	if (Input::IsKeyDown(KeyCode::KEY_A))
	{
		NINO_WARN("A pressed!");
	}
	if (Input::IsKeyUp(KeyCode::KEY_A))
	{
		NINO_WARN("A released!");
	}
	if (Input::IsMouseDown(MouseCode::MOUSE_LEFT))
	{
		NINO_WARN("Mouse left pressed!");
	}
	if (Input::IsMouseUp(MouseCode::MOUSE_RIGHT, &x, &y))
	{
		NINO_WARN("Mouse right released at {}, {}!", x, y);
	}
	if (Input::IsMouseScrolled(&offset, &x, &y))
	{
		NINO_WARN("Mouse scrolled by {} at {}, {}!", offset, x, y);
	}
}

void ApplicationLayer::RenderUserInterface()
{

}

void ApplicationLayer::OnEvent(Event& e)
{
	
}
