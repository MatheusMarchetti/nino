#pragma once

#include "Core.h"
#include "GraphicsCore.h"

namespace nino
{
	class GraphicsAPI;
}

namespace nino
{
	class CORE_API Shader
	{
	public:
		Shader(std::wstring filepath, GraphicsAPI* graphicsAPI);
		~Shader() {}

		void SetLayout();

		void Release();

	private:
		std::string LoadShaderFromFile(std::wstring path);
		ID3DBlob* CompileFromFile(std::wstring path, LPCSTR profile);

	private:
		std::wstring m_FilePath;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
		ID3DBlob* m_vertexBlob;
		ID3DBlob* m_pixelBlob;

		GraphicsAPI* m_GraphicsAPI;
	};
}