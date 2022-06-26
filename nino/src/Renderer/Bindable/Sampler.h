#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Bindable.h"

namespace nino
{
	class Sampler : public Bindable
	{
	public:
		Sampler();

		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
	};
}