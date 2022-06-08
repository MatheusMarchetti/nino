#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Bindable.h"

namespace nino
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(const std::vector<unsigned short>& indices);

		virtual void Bind() override;
		UINT GetCount() { return m_Count; }

	private:
		UINT m_Count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
	};
}