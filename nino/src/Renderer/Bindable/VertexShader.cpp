#include "corepch.h"
#include "VertexShader.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	using namespace Microsoft::WRL;

	VertexShader::VertexShader(const std::string& filePath)
		: Shader(filePath)
	{
		auto device = GraphicsAPI::GetDevice();
		ComPtr<ID3DBlob> errorBlob;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef CORE_DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif

		if (m_ShaderFilePath.empty())
			m_ShaderFilePath = "Assets/Shaders/DefaultShader.hlsl";

		ThrowOnError(D3DCompileFromFile(m_ShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSmain", "vs_5_0", flags, NULL, &m_ShaderBlob, &errorBlob));

		if (errorBlob.Get())
		{
			throw std::exception((char*)errorBlob->GetBufferPointer());
		}

		ThrowOnError(device->CreateVertexShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_VertexShader));
	}

	void VertexShader::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	}
}

