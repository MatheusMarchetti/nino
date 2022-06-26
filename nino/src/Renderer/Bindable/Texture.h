#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Bindable.h"

namespace nino
{
	class Texture : public Bindable
	{
	public:
		Texture(const std::string& filepath, const uint32_t& slot = 0);

		virtual void Bind() override;

	private:
		uint32_t m_Slot;
		std::filesystem::path m_TexturePath;
		Microsoft::WRL::ComPtr<ID3D11Resource> m_Texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
	};
}