#pragma once

namespace nino
{
	class Material
	{
	public:
		Material(const std::string& albedoMap = {}, const std::string& bumpMap = {}, const std::string& metalnessMap = {}, const std::string& roughnessMap = {}, const std::string& AOMap = {}, const std::string& ShaderFile = {})
			: m_AlbedoTexture(albedoMap), m_NormalTexture(bumpMap), m_MetallicTexture(metalnessMap), m_RoughnessTexture(roughnessMap), m_AmbientOcclusionTexture(AOMap), m_ShaderFile(ShaderFile) {}

		const std::string& GetAlbedoMap() const			{ return m_AlbedoTexture; }
		const std::string& GetNormalMap() const			{ return m_NormalTexture; }
		const std::string& GetMetallicMap() const		{ return m_MetallicTexture; }
		const std::string& GetRoughnessMap() const		{ return m_RoughnessTexture; }
		const std::string& GetAOMap() const				{ return m_AmbientOcclusionTexture; }
		const std::string& GetShaderFile() const		{ return m_ShaderFile; }

	private:
		std::string m_AlbedoTexture;
		std::string m_NormalTexture;
		std::string m_MetallicTexture;
		std::string m_RoughnessTexture;
		std::string m_AmbientOcclusionTexture;
		std::string m_ShaderFile;
	};
}