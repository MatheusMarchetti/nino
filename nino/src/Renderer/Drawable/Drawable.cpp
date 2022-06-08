#include "corepch.h"
#include "Drawable.h"

#include "Renderer/GraphicsAPI/GraphicsAPI.h"

namespace nino
{
	void Drawable::Draw()
	{
		auto context = GraphicsAPI::GetContext();

		for (const auto& bindable : m_Bindables)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(bindable.get()))
			{
				m_IndexCount = p->GetCount();
			}

			bindable->Bind();
		}

		for (const auto& staticBindable : GetStaticBindables())
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(staticBindable.get()))
			{
				m_IndexCount = p->GetCount();
			}

			staticBindable->Bind();
		}

		GraphicsAPI::BindTargets();

		ThrowInfoMessage(context->DrawIndexed(m_IndexCount, 0, 0));
	}

	void Drawable::SetData(const ModelCBuf data)
	{
		m_ModelTransform->SetData(data);
	}

	void Drawable::AddBind(const Ref<Bindable>& bind)
	{
		m_Bindables.push_back(bind);
	}
}

