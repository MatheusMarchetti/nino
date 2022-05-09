#include "corepch.h"
#include "LayerStack.h"

namespace nino
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::AttachLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerIndex, layer);
		m_LayerIndex++;
		layer->OnAttach();
	}

	void LayerStack::AttachOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::DetachLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerIndex, layer);

		if (it != m_Layers.begin() + m_LayerIndex)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerIndex--;
		}
	}

	void LayerStack::DetachOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin() + m_LayerIndex, m_Layers.end(), overlay);

		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}
}
