#include "corepch.h"
#include "D3D11Shader.h"

namespace nino
{
	D3D11VertexShader::D3D11VertexShader(const std::string& filePath)
		: Shader(filePath, ShaderType::VertexShader)
	{
		auto device = D3D11Backend::GetDevice();
		ThrowOnError(device->CreateVertexShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_VertexShader));
	}

	D3D11PixelShader::D3D11PixelShader(const std::string& filePath)
		: Shader(filePath, ShaderType::PixelShader)
	{
		auto device = D3D11Backend::GetDevice();
		ThrowOnError(device->CreatePixelShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_PixelShader));
	}

	D3D11ComputeShader::D3D11ComputeShader(const std::string& filePath)
		: Shader(filePath, ShaderType::ComputeShader)
	{
		auto device = D3D11Backend::GetDevice();
		ThrowOnError(device->CreateComputeShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_ComputeShader));
	}
}