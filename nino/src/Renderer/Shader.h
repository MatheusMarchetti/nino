#pragma once

#include "Assets/Asset.h"

#include "Core/Core.h"
#include "Core/UUID.h"

#include "Renderer/GraphicsAPI/GraphicsCore.h"

namespace nino
{
	class Shader : public Asset
	{
	protected:
		enum class ShaderType
		{
			VertexShader,
			PixelShader,
			ComputeShader
		};

	protected:
		Shader(const std::string& filePath, ShaderType type);
		virtual ~Shader() = default;

	public:
		ID3DBlob* GetShaderByteCode() { return m_ShaderBlob.Get(); }

	protected:
		std::filesystem::path m_ShaderFilePath;
		Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderBlob;
		inline static std::unordered_map<UUID, Shader*> s_ShaderCache;
	};
}