#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Bindable.h"

namespace nino
{
	enum class DepthOptions
	{
		LessEqual = D3D11_COMPARISON_LESS_EQUAL,
	};
	class DepthStencilState : public Bindable
	{
	public:
		DepthStencilState(DepthOptions depthOptions);

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	};
}