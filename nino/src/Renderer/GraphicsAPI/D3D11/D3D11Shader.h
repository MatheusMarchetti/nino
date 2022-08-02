#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/GraphicsAPI/D3D11/D3D11Backend.h"
#include "Renderer/Shader.h"

namespace nino
{
	class D3D11VertexShader : public Shader
	{
	public:
		D3D11VertexShader(const std::string& filePath);
		virtual ~D3D11VertexShader() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	};

	class D3D11PixelShader : public Shader
	{
	public:
		D3D11PixelShader(const std::string& filePath);
		virtual ~D3D11PixelShader() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	};

	class D3D11ComputeShader : public Shader
	{
	public:
		D3D11ComputeShader(const std::string& filePath);
		virtual ~D3D11ComputeShader() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ComputeShader;
	};
}