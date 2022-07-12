#include "ApplicationLayer.h"

#include "imgui.h"

using namespace nino;

void ApplicationLayer::OnAttach()
{
	auto nativeWindow = ParentApplication->GetNativeWindow();

	FramebufferDescriptor mainFrameDesc;
	mainFrameDesc.Name = "Native Window";
	mainFrameDesc.ColorResource = CreateRef<Texture>(nativeWindow);
	mainFrameDesc.DepthStencilResource = CreateRef<Texture>(TextureType::Texture2D, TextureUsage::DepthBinding, nativeWindow->GetWidth(), nativeWindow->GetHeight());
	mainFrameDesc.Height = nativeWindow->GetHeight();
	mainFrameDesc.Width = nativeWindow->GetWidth();

	m_MainFramebuffer = CreateRef<Framebuffer>(mainFrameDesc);
}

void ApplicationLayer::OnUpdate(Timestep ts)
{
	float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	m_MainFramebuffer->Clear(clearColor, 1.0f);

	RenderManager::BeginScene();
	{
		float position[] = { 0.0f, 1.0f, 0.0f };
		float rotation[] = { angle, angle, angle };
		float scale[] = { 0.5f, 0.5f, 0.5f };
		float color[] = { 0.5f * sinf(angle), 0.3f * sinf(angle), 0.2f * sinf(angle), 1.0f };

		PrimitiveDescriptor cube1 = {};
		cube1.Type = PrimitiveType::Cube;
		cube1.Position = position;
		cube1.Rotation = rotation;
		cube1.Scale = scale;
		cube1.Color = color;

		RenderManager::DrawPrimitive(cube1);
	}
	RenderManager::EndScene({ m_MainFramebuffer });

	angle += angle * ts;
}

void ApplicationLayer::RenderUserInterface()
{

}

void ApplicationLayer::OnEvent(Event& e)
{
	
}
