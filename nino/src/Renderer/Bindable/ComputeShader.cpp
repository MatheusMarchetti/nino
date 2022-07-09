#include "corepch.h"
#include "ComputeShader.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	using namespace Microsoft::WRL;

	ComputeShader::ComputeShader(const std::string& filePath)
		: Shader(filePath)
	{
		auto device = GraphicsAPI::GetDevice();
		ComPtr<ID3DBlob> errorBlob;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef CORE_DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif

		if (m_ShaderFilePath.empty())
			throw std::exception("Undefined Compute Shader.");

		std::filesystem::path csoShaderPath = m_ShaderFilePath.parent_path().string() + "/Cache/" + m_ShaderFilePath.filename().string();

		csoShaderPath = csoShaderPath.replace_extension().string() + "_CS";

		std::filesystem::directory_entry csoShader{ csoShaderPath.replace_extension(".cso") };

		if (csoShader.exists())
			m_ShaderFilePath = csoShaderPath;

		if (m_ShaderFilePath.extension() == ".cso")
		{
			ThrowOnError(D3DReadFileToBlob(m_ShaderFilePath.c_str(), &m_ShaderBlob));
		}

		else if (m_ShaderFilePath.extension() == ".hlsl")
		{
			D3DCompileFromFile(m_ShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CSmain", "cs_5_0", flags, NULL, &m_ShaderBlob, &errorBlob);

			if (errorBlob.Get())
			{
				std::string error = (char*)errorBlob->GetBufferPointer();
				throw std::exception((char*)errorBlob->GetBufferPointer());
			}

			m_ShaderFilePath = csoShaderPath;

			std::ofstream csoFile(m_ShaderFilePath, std::ios::out | std::ios::binary);
			if (!csoFile)
			{
				throw std::exception("Could not create Compute Shader CSO file");
			}

			csoFile.write(static_cast<const char*>(m_ShaderBlob->GetBufferPointer()), m_ShaderBlob->GetBufferSize());
			csoFile.close();
		}

		ThrowOnError(device->CreateComputeShader(m_ShaderBlob->GetBufferPointer(), m_ShaderBlob->GetBufferSize(), nullptr, &m_ComputeShader));
	}

	void ComputeShader::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->CSSetShader(m_ComputeShader.Get(), nullptr, 0);
	}

	void ComputeShader::SetResources(const Microsoft::WRL::ComPtr<ID3D11Buffer>& ConstantBuffer, const Microsoft::WRL::ComPtr<ID3D11SamplerState>& Sampler, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& SRV, const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& UAV)
	{
		auto context = GraphicsAPI::GetContext();

		context->CSSetConstantBuffers(0, 1, ConstantBuffer.GetAddressOf());
		context->CSSetSamplers(0, 1, Sampler.GetAddressOf());
		context->CSSetShaderResources(0, 1, SRV.GetAddressOf());
		context->CSSetUnorderedAccessViews(0, 1, UAV.GetAddressOf(), nullptr);
	}

	void ComputeShader::Run(uint32_t Thread_X, uint32_t Thread_Y, uint32_t Thread_Z)
	{
		auto context = GraphicsAPI::GetContext();

		context->Dispatch(Thread_X, Thread_Y, Thread_Z);
	}
}
