#pragma once

#include "Core/Core.h"

#include "Renderer/Texture.h"
#include "Renderer/Shader.h"

namespace nino
{
	class Material
	{
	public:
		Material(const std::string& albedo, const std::string& shader, const std::string& normal = {}, const std::string& metalness = {}, const std::string& roughness = {});
		~Material() = default;

		void SetTilingFactor(float tilingFactor) { m_TilingFactor = tilingFactor; }
		void SetBaseColor(const Color& color) { m_BaseColor = color; }

		Ref<Texture> GetAlbedoMap() { return m_AlbedoMap; }
		Ref<Texture> GetNormalMap() { return m_NormalMap; }
		Ref<Texture> GetMetalnessMap() { return m_MetalnessMap; }
		Ref<Texture> GetRoughnessMap() { return m_RoughnessMap; }

	private:
		float m_TilingFactor;
		Color m_BaseColor;
		Ref<Texture> m_AlbedoMap;
		Ref<Texture> m_NormalMap;
		Ref<Texture> m_MetalnessMap;
		Ref<Texture> m_RoughnessMap;
		Ref<PixelShader> m_Shader;
	};
}