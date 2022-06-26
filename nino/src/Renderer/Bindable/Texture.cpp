#include "corepch.h"
#include "Texture.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

#include "WICTextureLoader.h"
#include "DirectXTex.h"

namespace nino
{
	using namespace DirectX;
	Texture::Texture(const std::string& filepath, const uint32_t& slot)
		: m_TexturePath(filepath), m_Slot(slot)
	{
		auto device = GraphicsAPI::GetDevice();

		ThrowOnError(CreateWICTextureFromFile(device.Get(), m_TexturePath.c_str(), &m_Texture, &m_ShaderResourceView));
	}

	void Texture::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->PSSetShaderResources(m_Slot, 1, m_ShaderResourceView.GetAddressOf());
	}
}

