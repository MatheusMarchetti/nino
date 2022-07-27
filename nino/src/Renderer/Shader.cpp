#include "corepch.h"
#include "Shader.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"
#include "Core/Timestep.h"

namespace nino
{
	using namespace Microsoft::WRL;

	Shader::Shader(const std::string& filePath, ShaderType type)
		: m_ShaderFilePath(filePath)
	{
		std::string appendType = "";
		std::string entryPoint = "";
		std::string shaderModel = "";

		switch (type)
		{
		case nino::Shader::ShaderType::VertexShader:
		{
			appendType = "_VS";
			entryPoint = "VSmain";
			shaderModel = "vs_5_0";
			break;
		}
		case nino::Shader::ShaderType::PixelShader:
		{
			appendType = "_PS";
			entryPoint = "PSmain";
			shaderModel = "ps_5_0";
			break;
		}
		case nino::Shader::ShaderType::ComputeShader:
		{
			appendType = "_CS";
			entryPoint = "CSmain";
			shaderModel = "cs_5_0";
			break;
		}
		default:
			throw std::exception("Unknown shader type");
		}

		auto device = GraphicsAPI::GetDevice();
		ComPtr<ID3DBlob> errorBlob;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef CORE_DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif

		std::filesystem::path csoShaderPath = m_ShaderFilePath;
		std::hash<std::string> hasher;

		csoShaderPath = csoShaderPath.replace_extension().string() + appendType;

		csoShaderPath = std::to_string(hasher(csoShaderPath.string()));

		std::filesystem::directory_entry csoShader{ csoShaderPath.replace_extension(".cso") };

		if (csoShader.exists() && std::filesystem::last_write_time(m_ShaderFilePath) < std::filesystem::last_write_time(csoShaderPath))
			m_ShaderFilePath = csoShaderPath;

		Timestep shaderTimer;

		if (m_ShaderFilePath.extension() == ".cso")
		{
			shaderTimer.Tick();
			ThrowOnError(D3DReadFileToBlob(m_ShaderFilePath.c_str(), &m_ShaderBlob));
			shaderTimer.Tick();

			NINO_CORE_INFO("Shader loading took: {} ms", shaderTimer.GetMilliseconds());
		}

		else if (m_ShaderFilePath.extension() == ".hlsl")
		{
			shaderTimer.Tick();
			ThrowOnError(D3DCompileFromFile(m_ShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), shaderModel.c_str(), flags, NULL, &m_ShaderBlob, &errorBlob));
			shaderTimer.Tick();

			if (errorBlob.Get())
			{
				throw std::exception((char*)errorBlob->GetBufferPointer());
			}

			NINO_CORE_INFO("Shader compilation took: {} ms", shaderTimer.GetMilliseconds());

			m_ShaderFilePath = csoShaderPath;

			std::ofstream csoFile(m_ShaderFilePath, std::ios::out | std::ios::binary);
			if (!csoFile)
			{
				throw std::exception("Could not create CSO file");
			}

			csoFile.write(static_cast<const char*>(m_ShaderBlob->GetBufferPointer()), m_ShaderBlob->GetBufferSize());
			csoFile.close();
		}

		else
		{
			throw std::exception("Unsupported shader file format. Only supports HLSL or CSO files");
		}
	}

	VertexShader::VertexShader(const std::string& filePath)
		: Shader(filePath, ShaderType::VertexShader)
	{
		auto device = GraphicsAPI::GetDevice();

		ThrowOnError(device->CreateVertexShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_VertexShader));
	}

	PixelShader::PixelShader(const std::string& filePath)
		: Shader(filePath, ShaderType::PixelShader)
	{
		auto device = GraphicsAPI::GetDevice();

		ThrowOnError(device->CreatePixelShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_PixelShader));
	}

	ComputeShader::ComputeShader(const std::string& filePath)
		: Shader(filePath, ShaderType::ComputeShader)
	{
		auto device = GraphicsAPI::GetDevice();

		ThrowOnError(device->CreateComputeShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_ComputeShader));
	}
}