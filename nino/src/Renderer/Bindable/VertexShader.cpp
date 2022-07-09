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

		std::filesystem::path csoShaderPath = m_ShaderFilePath.parent_path().string() + "/Cache/" + m_ShaderFilePath.filename().string();

		csoShaderPath = csoShaderPath.replace_extension().string() + "_VS";

		std::filesystem::directory_entry csoShader{ csoShaderPath.replace_extension(".cso") };

		if (csoShader.exists())
			m_ShaderFilePath = csoShaderPath;

		if (m_ShaderFilePath.extension() == ".cso")
		{
			ThrowOnError(D3DReadFileToBlob(m_ShaderFilePath.c_str(), &m_ShaderBlob));
		}

		else if (m_ShaderFilePath.extension() == ".hlsl")
		{
			D3DCompileFromFile(m_ShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSmain", "vs_5_0", flags, NULL, &m_ShaderBlob, &errorBlob);

			if (errorBlob.Get())
			{
				std::string error = (char*)errorBlob->GetBufferPointer();
				throw std::exception((char*)errorBlob->GetBufferPointer());
			}

			m_ShaderFilePath = csoShaderPath;

			std::ofstream csoFile(m_ShaderFilePath, std::ios::out | std::ios::binary);
			if (!csoFile)
			{
				throw std::exception("Could not create Vertex Shader CSO file");
			}

			csoFile.write(static_cast<const char*>(m_ShaderBlob->GetBufferPointer()), m_ShaderBlob->GetBufferSize());
			csoFile.close();
		}

		ThrowOnError(device->CreateVertexShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_VertexShader));
	}

	void VertexShader::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	}
}

