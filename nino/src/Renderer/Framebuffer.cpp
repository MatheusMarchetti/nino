#include "corepch.h"
#include "Framebuffer.h"

#ifdef NINO_API_D3D11
#include "Renderer/GraphicsAPI/D3D11/D3D11Framebuffer.h"
#else

#endif

namespace nino
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferDescriptor& descriptor)
    {
#ifdef NINO_API_D3D11
        return CreateRef<D3D11Framebuffer>(descriptor);
#else
        
#endif
    }
}
