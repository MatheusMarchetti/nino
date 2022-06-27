#include "corepch.h"
#include "PixelShader.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	using namespace Microsoft::WRL;

	PixelShader::PixelShader(const std::string& filePath)
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

		std::filesystem::path csoShaderPath = m_ShaderFilePath;

		csoShaderPath = csoShaderPath.replace_extension().string() + "_PS";

		std::filesystem::directory_entry csoShader{ csoShaderPath.replace_extension(".cso") };

		if (csoShader.exists())
			m_ShaderFilePath = csoShaderPath;

		if (m_ShaderFilePath.extension() == ".cso")
		{
			ThrowOnError(D3DReadFileToBlob(m_ShaderFilePath.c_str(), &m_ShaderBlob));
		}

		else if(m_ShaderFilePath.extension() == ".hlsl")
		{
			ThrowOnError(D3DCompileFromFile(m_ShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSmain", "ps_5_0", flags, NULL, &m_ShaderBlob, &errorBlob));

			if (errorBlob.Get())
			{
				throw std::exception((char*)errorBlob->GetBufferPointer());
			}

			m_ShaderFilePath = csoShaderPath;

			std::ofstream csoFile(m_ShaderFilePath, std::ios::out | std::ios::binary);
			if (!csoFile)
			{
				throw std::exception("Could not create Pixel Shader CSO file");
			}

			csoFile.write(static_cast<const char*>(m_ShaderBlob->GetBufferPointer()), m_ShaderBlob->GetBufferSize());
			csoFile.close();
		}

		else
		{
			throw std::exception("Unsupported shader file format. Only supports HLSL or CSO files");
		}

		ThrowOnError(device->CreatePixelShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_PixelShader));
	}

	void PixelShader::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	}
}

