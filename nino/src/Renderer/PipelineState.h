#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"

namespace nino
{
	enum class ComparisonFunction
	{
		Never = D3D11_COMPARISON_NEVER,
		Always = D3D11_COMPARISON_ALWAYS,
		Less = D3D11_COMPARISON_LESS,
		Equal = D3D11_COMPARISON_EQUAL,
		Greater = D3D11_COMPARISON_GREATER,
		Less_Equal = D3D11_COMPARISON_LESS_EQUAL,
		Greater_Equal = D3D11_COMPARISON_GREATER_EQUAL,
		Not_Equal = D3D11_COMPARISON_NOT_EQUAL
	};

	class SamplerState
	{
	public:
		enum class Filter
		{
			Point = D3D11_FILTER_MIN_MAG_MIP_POINT,
			Bilinear = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
			Trilinear = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			Anisotropic = D3D11_FILTER_ANISOTROPIC,
		};

		enum class TextureMode
		{
			Clamp = D3D11_TEXTURE_ADDRESS_CLAMP,
			Wrap = D3D11_TEXTURE_ADDRESS_WRAP,
			Border = D3D11_TEXTURE_ADDRESS_BORDER
		};

	public:
		SamplerState(Filter samplingFilter, TextureMode textureMode, ComparisonFunction comparisonFunc, uint32_t anisotropyLevel = D3D11_MAX_MAXANISOTROPY, const Color& borderColor = {});
		~SamplerState() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
	};

	class RasterizerState
	{
	public:
		enum class FillMode
		{
			Wireframe = D3D11_FILL_WIREFRAME,
			Solid = D3D11_FILL_SOLID
		};

		enum class CullMode
		{
			None = D3D11_CULL_NONE,
			Front = D3D11_CULL_FRONT,
			Back = D3D11_CULL_BACK
		};

	public:
		RasterizerState(FillMode fillMode, CullMode cullMode);
		~RasterizerState() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerState;
	};

	class DepthStencilState
	{
	public:
		DepthStencilState(bool depthEnabled, bool depthWrite, ComparisonFunction comparisonFunc, bool stencilEnabled, uint8_t stencilRead, uint8_t stencilWrite);
		~DepthStencilState() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	};

	class BlendState
	{
	public:
		BlendState() = default;
		~BlendState() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendState;
	};

	class PrimitiveTopology
	{
	public:
		enum class TopologyType
		{
			TriangleList = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			TriangleStrip = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			LineList = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			LineStrip = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
		};

	public:
		PrimitiveTopology(TopologyType type);
		~PrimitiveTopology() = default;
	};

	class InputLayout
	{
	public:
		enum class ShaderDataType
		{
			Uint_8b = DXGI_FORMAT_R8_UINT,
			Uint_8bU = DXGI_FORMAT_R8_UNORM,
			Uint2_8b = DXGI_FORMAT_R8G8_UINT,
			Uint2_8bU = DXGI_FORMAT_R8G8_UNORM,
			Uint4_8b = DXGI_FORMAT_R8G8B8A8_UINT,
			Uint4_8bU = DXGI_FORMAT_R8G8B8A8_UNORM,
			Uint_16b = DXGI_FORMAT_R16_UINT,
			Uint_16bU = DXGI_FORMAT_R16_UNORM,
			Float2_16b = DXGI_FORMAT_R16G16_FLOAT,
			Float4_16b = DXGI_FORMAT_R16G16B16A16_FLOAT,
			Uint_32b = DXGI_FORMAT_R32_UINT,
			Float_32b = DXGI_FORMAT_R32_FLOAT,
			Float2_32b = DXGI_FORMAT_R32G32_FLOAT,
			Float3_32b = DXGI_FORMAT_R32G32B32_FLOAT,
			Float4_32b = DXGI_FORMAT_R32G32B32A32_FLOAT,
		};

		enum class ElementClassification
		{
			Vertex = D3D11_INPUT_PER_VERTEX_DATA,
			Instance = D3D11_INPUT_PER_INSTANCE_DATA
		};

		struct BufferElement
		{
			const char* Name;
			ShaderDataType DataType;
			ElementClassification Classification;
			uint32_t InstanceSize;

			BufferElement() = default;
			BufferElement(const char* name, ShaderDataType type, ElementClassification classification = ElementClassification::Vertex, uint32_t instanceSize = 0)
				: Name(name), DataType(type), Classification(classification), InstanceSize(instanceSize)
			{}
		};

		class BufferLayout
		{
		public:
			BufferLayout() = default;
			BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements) {}

			std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
			std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
			std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
			std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		private:
			std::vector<BufferElement> m_Elements;
		};

	public:
		InputLayout(ID3DBlob* VertexShaderBytecode, const BufferLayout& layout);
		~InputLayout() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}