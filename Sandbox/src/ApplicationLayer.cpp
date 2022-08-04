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

	}
	RenderManager::EndScene();
}

void ApplicationLayer::RenderUserInterface()
{

}

void ApplicationLayer::OnEvent(Event& e)
{
	
}
