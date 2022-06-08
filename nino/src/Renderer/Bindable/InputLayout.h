#pragma once

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/Bindable.h"

namespace nino
{
	enum class ShaderDataType
	{
		None = 0,
		Uint_8b,
		Uint2_8b,
		Uint4_8b,
		Uint_16b,
		Float2_16b,
		Float4_16b,
		Uint_32b,
		Float_32b,
		Float2_32b,
		Float3_32b,
		Float4_32b,
	};

	enum class ElementClassification
	{
		None = 0,
		Vertex,
		Instance
	};

	struct BufferElement
	{
		const char* Name;
		ShaderDataType DataType;
		ElementClassification Classification;
		uint32_t InstanceSize;
		bool Normalized;

		BufferElement() = default;
		BufferElement(const char* name, ShaderDataType type, ElementClassification classification = ElementClassification::Vertex, uint32_t instanceSize = 0, bool normalized = false)
			: Name(name), DataType(type), Classification(classification), InstanceSize(instanceSize), Normalized(normalized)
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

	class InputLayout : public Bindable
	{
	public:
		InputLayout(ID3DBlob* shaderByteCode, const BufferLayout& layout);

		D3D11_INPUT_CLASSIFICATION ElementClassificationType(ElementClassification classification);
		DXGI_FORMAT ShaderDataTypeSize(ShaderDataType type, bool normalized);
		virtual void Bind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}