#include "corepch.h"
#include "VertexBuffer.h"

namespace nino
{
	void VertexBuffer::Bind()
	{
		auto context = GraphicsAPI::GetContext();

		context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_Stride, &m_Offset);
	}
}

