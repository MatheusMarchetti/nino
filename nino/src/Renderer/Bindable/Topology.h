#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Bindable.h"

namespace nino
{
	enum class TopologyType
	{
		None = 0,
		TriangleList,
		TriangleStrip,
		LineList,
		LineStrip
	};

	class Topology : public Bindable
	{
	public:
		Topology(TopologyType type);

		virtual void Bind() override;
		D3D11_PRIMITIVE_TOPOLOGY GetTopologyType(TopologyType type);

	private:
		D3D11_PRIMITIVE_TOPOLOGY m_TopologyType;
	};
}