#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Shader.h"

namespace nino
{
	class PixelShader : public Shader
	{
	public:
		PixelShader(const std::string& filePath);
		virtual ~PixelShader() = default;

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	};
}
