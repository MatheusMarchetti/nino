#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Shader.h"

namespace nino
{
	class VertexShader : public Shader
	{
	public:
		VertexShader();
		VertexShader(const std::string& filePath);
		virtual ~VertexShader() = default;

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	};
}