#include "corepch.h"
#include "Material.h"

#include "Assets/AssetManager.h"

namespace nino
{
	Material::Material(const std::string& albedo, const std::string& shader, const std::string& normal, const std::string& metalness, const std::string& roughness)
		: m_TilingFactor(0.0f), m_BaseColor()
	{
		if(!albedo.empty())
			AssetManager::LoadAsset(albedo, m_AlbedoMap);
		if(!normal.empty())
			AssetManager::LoadAsset(normal, m_NormalMap);
		if(!metalness.empty())
			AssetManager::LoadAsset(metalness, m_MetalnessMap);
		if(!roughness.empty())
			AssetManager::LoadAsset(roughness, m_RoughnessMap);

		AssetManager::LoadAsset(shader, m_Shader);
	}
}

