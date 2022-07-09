#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Shader.h"

namespace nino
{
	class ComputeShader : public Shader
	{
	public:
		ComputeShader(const std::string& filePath);
		virtual ~ComputeShader() = default;

		virtual void Bind() override;
		
		void SetResources(const Microsoft::WRL::ComPtr<ID3D11Buffer>& ConstantBuffer = nullptr, const Microsoft::WRL::ComPtr<ID3D11SamplerState>& Sampler = nullptr, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& SRV = nullptr, const Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& UAV = nullptr);
		void Run(uint32_t Thread_X, uint32_t Thread_Y, uint32_t Thread_Z);

	private:
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ComputeShader;
	};
}
