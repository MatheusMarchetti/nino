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

	Ref<Texture> test;

	AssetManager::LoadAsset("Assets/Textures/Bricks.jpg", test);
}

void ApplicationLayer::OnUpdate(Timestep ts)
{
	Color clearColor = { 0.4f, 0.5f, 0.8f, 1.0f };

	m_MainFramebuffer->Clear(clearColor, 1.0f);

	RenderManager::DrawGrid(5.0f, 5.0f);

	RenderManager::BeginScene();
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

		RenderManager::DrawPrimitive(cube1);
	}
	RenderManager::EndScene({ m_MainFramebuffer });

	angle += ts;
}

void ApplicationLayer::RenderUserInterface()
{

}

void ApplicationLayer::OnEvent(Event& e)
{
	
}
