#include "corepch.h"
#include "D3D11Shader.h"

namespace nino
{
	auto device = D3D11Backend::GetDevice();

	D3D11VertexShader::D3D11VertexShader(const std::string& filePath)
		: Shader(filePath, ShaderType::VertexShader)
	{
		ThrowOnError(device->CreateVertexShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_VertexShader));
	}

	D3D11PixelShader::D3D11PixelShader(const std::string& filePath)
		: Shader(filePath, ShaderType::PixelShader)
	{
		ThrowOnError(device->CreatePixelShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_PixelShader));
	}

	D3D11ComputeShader::D3D11ComputeShader(const std::string& filePath)
		: Shader(filePath, ShaderType::ComputeShader)
	{
		ThrowOnError(device->CreateComputeShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_ComputeShader));
	}
}