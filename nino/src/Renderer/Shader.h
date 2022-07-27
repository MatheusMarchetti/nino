#pragma once

#include "Core/Core.h"
#include "Core/UUID.h"

#include "Renderer/GraphicsAPI/GraphicsCore.h"

namespace nino
{
	class Shader : public std::enable_shared_from_this<Shader>
	{
	protected:
		enum class ShaderType
		{
			VertexShader,
			PixelShader,
			ComputeShader
		};

	public:
		Shader() = default;
		Shader(const std::string& filePath, ShaderType type);
		virtual ~Shader() = default;

		ID3DBlob* GetShaderByteCode() { return m_ShaderBlob.Get(); }
		UUID GetUUID() { return m_UUID; }

	protected:
		UUID m_UUID;
		std::filesystem::path m_ShaderFilePath;
		Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderBlob;
		inline static std::unordered_map<UUID, Ref<Shader>> s_ShaderCache;
	};

	class VertexShader : public Shader
	{
	public:
		VertexShader(const std::string& filePath);
		virtual ~VertexShader() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	};

	class PixelShader : public Shader
	{
	public:
		PixelShader(const std::string& filePath);
		virtual ~PixelShader() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	};

	class ComputeShader : public Shader
	{
	public:
		ComputeShader(const std::string& filePath);
		virtual ~ComputeShader() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ComputeShader;
	};
}