#include "corepch.h"
#include "Shader.h"

#include "Renderer/GraphicsAPI.h"

namespace nino
{
	Shader::Shader(std::wstring filepath, GraphicsAPI* graphicsAPI)
		: m_FilePath(filepath), m_GraphicsAPI(graphicsAPI)
	{
		auto device = m_GraphicsAPI->GetDevice();
		auto context = m_GraphicsAPI->GetContext();

		m_vertexBlob = CompileFromFile(L"C:/dev/Game Engine/nino/Sandbox/src/Assets/FlatColorVS.hlsl", "vs_5_0");
		m_pixelBlob = CompileFromFile(L"C:/dev/Game Engine/nino/Sandbox/src/Assets/FlatColorPS.hlsl", "ps_5_0");

		device->CreateVertexShader(m_vertexBlob->GetBufferPointer(), m_vertexBlob->GetBufferSize(), nullptr, &m_VertexShader);
		device->CreatePixelShader(m_pixelBlob->GetBufferPointer(), m_pixelBlob->GetBufferSize(), nullptr, &m_PixelShader);

		context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
		context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	}

	void Shader::SetLayout()
	{
		auto device = m_GraphicsAPI->GetDevice();
		auto context = m_GraphicsAPI->GetContext();

		D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), m_vertexBlob->GetBufferPointer(), m_vertexBlob->GetBufferSize(), &m_InputLayout);
		context->IASetInputLayout(m_InputLayout.Get());
	}

	void Shader::Release()
	{
		m_pixelBlob->Release();
		m_vertexBlob->Release();

		m_InputLayout = nullptr;
		m_VertexShader = nullptr;
		m_PixelShader = nullptr;

		m_GraphicsAPI = nullptr;
	}

	ID3DBlob* Shader::CompileFromFile(std::wstring path, LPCSTR profile)
	{
		char* entryPoint = "main";
		ID3DBlob* shaderBlob;
		ID3DBlob* errorBlob;

		HRESULT hr = D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, profile, 0, 0, &shaderBlob, &errorBlob);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}

			if (shaderBlob)
				shaderBlob->Release();

			return nullptr;
		}

		return shaderBlob;
	}
}

