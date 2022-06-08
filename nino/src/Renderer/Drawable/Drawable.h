#pragma once

#include "Core/Core.h"

#include "Renderer/GraphicsAPI/GraphicsCore.h"
#include "Renderer/Bindable/BindableCore.h"

namespace nino
{
	class Drawable
	{
		template<typename T>
		friend class DrawableBase;

	public:
		virtual ~Drawable() = default;

		void Draw();
		void SetData(const ModelCBuf data);
		void AddBind(const Ref<Bindable>& bind);

	private:
		virtual const std::vector<Scope<Bindable>>& GetStaticBindables() = 0;

	protected:
		uint32_t m_IndexCount;
		Ref<ModelConstantBuffer> m_ModelTransform;
		std::vector<Ref<Bindable>> m_Bindables;
	};
}