#pragma once

#include "Core/Core.h"

#include "Renderer/Drawable/Drawable.h"
#include "Renderer/Bindable/IndexBuffer.h"

namespace nino
{
	template<typename T>
	class DrawableBase : public Drawable
	{
	protected:
		bool IsStaticInitialized() { return !m_StaticBindables.empty(); }
		static void AddStaticBind(Scope<Bindable> bind)
		{
			m_StaticBindables.push_back(std::move(bind));
		}
		void SetIndexFromStatic()
		{
			for (const auto& binds : m_StaticBindables)
			{
				if (const auto p = dynamic_cast<IndexBuffer*>(binds.get()))
				{
					m_IndexCount = p->GetCount();
					return;
				}
			}
		}

	private:
		virtual const std::vector<Scope<Bindable>>& GetStaticBindables() override { return m_StaticBindables; }

	private:
		static std::vector<Scope<Bindable>> m_StaticBindables;
	};

	template<typename T>
	std::vector<Scope<Bindable>> DrawableBase<T>::m_StaticBindables;
}