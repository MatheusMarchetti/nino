#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Shader.h"

namespace nino
{
	class VertexShader : public Shader
	{
	public:
		VertexShader(const std::string& filePath = "Assets/Shaders/DefaultShader.hlsl");
		virtual ~VertexShader() = default;

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	};
}