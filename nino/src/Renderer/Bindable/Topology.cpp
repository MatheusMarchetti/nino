#include "corepch.h"
#include "Topology.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
    Topology::Topology(TopologyType type)
    {
        m_TopologyType = GetTopologyType(type);
    }

    void Topology::Bind()
    {
        auto context = GraphicsAPI::GetContext();

        context->IASetPrimitiveTopology(m_TopologyType);
    }

    D3D11_PRIMITIVE_TOPOLOGY Topology::GetTopologyType(TopologyType type)
    {
        switch (type)
        {
            case TopologyType::TriangleList:    return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            case TopologyType::TriangleStrip:   return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
            case TopologyType::LineList:        return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
            case TopologyType::LineStrip:       return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
        }
    }
}

