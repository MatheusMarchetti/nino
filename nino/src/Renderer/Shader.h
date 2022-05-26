#pragma once

#include "Renderer/GraphicsCore.h"

namespace nino
{
	class GraphicsAPI;
}

namespace nino
{
	class Shader
	{
	public:
		Shader(std::wstring filepath, GraphicsAPI* graphicsAPI);
		~Shader() {}

		void SetLayout();

		void Release();

	private:
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