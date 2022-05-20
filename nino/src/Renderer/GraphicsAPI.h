#pragma once

#include "Core.h"
#include "Renderer/GraphicsCore.h"

namespace nino
{
	class CORE_API GraphicsAPI
	{
	public:
		GraphicsAPI();

		std::wstring GetAdapterName() { return m_SelectedAdapterName; }

		void CreateRootSignature();
		void CreatePipelineState();

		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature() { return m_RootSignature; }
		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState() { return m_PSO; }

		Microsoft::WRL::ComPtr<ID3D12Device2> GetDevice() { return m_Device; }

		void Release();

	private:
		Microsoft::WRL::ComPtr<IDXGIAdapter4> QueryAdapters();
		Microsoft::WRL::ComPtr<IDXGIAdapter4> SelectedAdapter();
		Microsoft::WRL::ComPtr<ID3D12Device2> CreateDevice();
		void EnableDebugLayer();

	private:
		bool m_DefaultAdapter = true;
		std::wstring m_SelectedAdapterName;

		std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<IDXGIAdapter4>> m_AvailableAdapters;

		Microsoft::WRL::ComPtr<IDXGIAdapter4> m_SelectedAdapter;
		Microsoft::WRL::ComPtr<ID3D12Device2> m_Device;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PSO;
	};
}