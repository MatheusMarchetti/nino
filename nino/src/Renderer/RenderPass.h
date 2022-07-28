#pragma once

#include "Core/Core.h"

#include "Renderer/Buffer.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/PipelineState.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

namespace nino
{
	class RenderPass
	{
	public:
		RenderPass(std::initializer_list<Ref<Framebuffer>> framebuffers) : m_Framebuffer(framebuffers) {}
		virtual ~RenderPass() = default;

	protected:
		std::vector<Ref<Framebuffer>> m_Framebuffer;
		std::vector<Ref<VertexShader>> m_VertexShader;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}