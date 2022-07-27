#include "corepch.h"
#include "AssetManager.h"

namespace nino
{
    AssetManager::AssetManager()
    {
    }

    void AssetManager::LoadAsset(const std::string& filePath, Ref<Texture>& texture)
    {
        std::filesystem::path assetFilePath = filePath;
        std::hash<std::string> hasher;

        std::string stringToHash = assetFilePath.filename().string() + std::to_string(std::filesystem::last_write_time(assetFilePath).time_since_epoch().count());

        UUID comparison;
        comparison = comparison.GenerateUUID(hasher(stringToHash));

        if (s_TextureCache.find(comparison) != s_TextureCache.end())
        {
            texture = s_TextureCache[comparison];
            texture->SetUUID(comparison);

            return;
        }
        DirectX::ScratchImage loadedImage;

        if (AssetLoader::HasAssetBinary(comparison))
        {
            loadedImage = AssetLoader::LoadTexture(comparison);
        }
        else
        {
            loadedImage = AssetLoader::LoadTexture(filePath);
            AssetLoader::CreateBinaryTexture(comparison, loadedImage);
        }
            
        TextureType type = TextureType::Texture2D;

        if (loadedImage.GetImageCount() > 1)
        {
            type = TextureType::Texture2DArray;

            if (loadedImage.GetImageCount() % 6 == 0)
                type = TextureType::TextureCube;
        }

        texture = CreateRef<Texture>(type, TextureUsage::ColorBinding, loadedImage);
        texture->SetUUID(comparison);

        s_TextureCache[comparison] = texture;
    }

    void AssetManager::LoadAsset(const std::string& filePath, Ref<PixelShader>& shader)
    {
        std::filesystem::path assetFilePath = filePath;
        std::hash<std::string> hasher;

        std::string stringToHash = assetFilePath.filename().string();

        UUID comparison;
        comparison = comparison.GenerateUUID(hasher(stringToHash));

        if (s_ShaderCache.find(comparison) != s_ShaderCache.end())
        {
            shader = s_ShaderCache[comparison];
            shader->SetUUID(comparison);

            return;
        }

        shader = CreateRef<PixelShader>(filePath);
        shader->SetUUID(comparison);

        s_ShaderCache[comparison] = shader;
    }

    void AssetManager::UnloadAsset(const Ref<Asset>& asset)
    {
        if (s_TextureCache.find(asset->GetUUID()) != s_TextureCache.end())
        {
            s_TextureCache.erase(asset->GetUUID());
        }
    }
}