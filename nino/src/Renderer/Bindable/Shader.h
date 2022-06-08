#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Bindable.h"

namespace nino
{
	class Shader : public Bindable
	{
	public:
		Shader(const std::string& filePath) : m_ShaderFilePath(filePath) {}
		virtual ~Shader() = default;

		ID3DBlob* GetByteCode() { return m_ShaderBlob.Get(); }

	protected:
		std::filesystem::path m_ShaderFilePath;
		Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderBlob;
	};
}