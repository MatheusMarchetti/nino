#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Bindable.h"

namespace nino
{
	enum class CullingOptions
	{
		None = D3D11_CULL_NONE,
		Back = D3D11_CULL_BACK,
		Front = D3D11_CULL_FRONT
	};
	class RasterizerState : public Bindable
	{
	public:
		RasterizerState(CullingOptions cullOptions);

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerState;
	};
}