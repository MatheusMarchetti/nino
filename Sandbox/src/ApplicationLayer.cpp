#include "ApplicationLayer.h"

#include "imgui.h"

using namespace nino;

void ApplicationLayer::OnAttach()
{
	auto nativeWindow = ParentApplication->GetNativeWindow();

	FramebufferDescriptor mainDescriptor;
	mainDescriptor.Name = "Main Framebuffer";
	mainDescriptor.ColorResource = Texture::Create(nativeWindow);
	mainDescriptor.DepthStencilResource = Texture::Create(TextureType::Texture2D, TextureUsage::DepthBinding, nativeWindow->GetWidth(), nativeWindow->GetHeight());
	mainDescriptor.ViewportSpecification =
	{
		0.0f, 0.0f,	//top-left x,y coordinates
		(float)nativeWindow->GetWidth(), (float)nativeWindow->GetHeight(),	//viewport dimensions
		0.0f, 1.0f	//min-max depth range
	};

	m_MainFramebuffer = Framebuffer::Create(mainDescriptor);

	m_MainCamera.ChangeCameraType(CameraType::Perspective);
	m_MainCamera.SetViewportSize((float)nativeWindow->GetWidth(), (float)nativeWindow->GetHeight());
	m_MainCamera.SetFieldOfView(45.0f);
}

void ApplicationLayer::OnUpdate(Timestep ts)
{
	Color clearColor = { 0.4f, 0.5f, 0.8f, 1.0f };

	m_MainFramebuffer->Clear(clearColor, 0.0f);

	RenderManager::BeginScene(m_MainCamera);
	{
		Vector3 position = { 0.0f, 1.0f, 0.0f };
		Vector3 rotation = { angle, angle, angle };
		Vector3 scale = { 0.5f, 0.5f, 0.5f };
		Vector4 color = { 0.2f * (sinf(angle) + 1.0f)/2.0f, 0.5f * (sinf(angle) + 1.0f) / 2.0f, 0.8f * (sinf(angle) + 1.0f) / 2.0f, 1.0f };

		Material testMaterial("Assets/Textures/environment.hdr", "Assets/Shaders/BasicPBR.hlsl");
	}
	RenderManager::EndScene();

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
