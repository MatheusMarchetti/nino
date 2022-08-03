#include "corepch.h"
#include "Texture.h"

#ifdef NINO_API_D3D11
#include "Renderer/GraphicsAPI/D3D11/D3D11Texture.h"
#else

#endif

namespace nino
{
    Ref<Texture> nino::Texture::Create(TextureType type, TextureUsage::TextureUsage usage, uint32_t width, uint32_t height, uint32_t arraySize)
    {
#ifdef NINO_API_D3D11
        return CreateRef<D3D11Texture>(type, usage, width, height, arraySize);
#else

#endif
    }
    
    Ref<Texture> nino::Texture::Create(TextureType type, TextureUsage::TextureUsage usage, DirectX::ScratchImage& image)
    {
#ifdef NINO_API_D3D11
        return CreateRef<D3D11Texture>(type, usage, image);
#else

#endif
    }
    
    Ref<Texture> nino::Texture::Create(const Window* window)
    {
#ifdef NINO_API_D3D11
        return CreateRef<D3D11Texture>(window);
#else

#endif
    }
}
