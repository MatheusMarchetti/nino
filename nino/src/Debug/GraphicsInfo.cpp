#include "corepch.h"
#include "GraphicsInfo.h"

namespace nino
{
	unsigned long long GraphicsInfo::m_NextMessage = 0;
	std::string GraphicsInfo::m_ErrorMessage;
	IDXGIInfoQueue* GraphicsInfo::m_InfoQueue = nullptr;

	void GraphicsInfo::Set()
	{
		typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

		const auto dxgiDebugModule = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (!dxgiDebugModule)
			throw std::exception("Failed to load dxgidebug.dll");

		const auto GetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(dxgiDebugModule, "DXGIGetDebugInterface")));

		if (!GetDebugInterface)
			throw std::exception("Failed to load symbols from dll");

		ThrowOnError(GetDebugInterface(IID_PPV_ARGS(&m_InfoQueue)));
		m_NextMessage = m_InfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}

	std::vector<std::string> GraphicsInfo::GetMessages()
	{
		std::vector<std::string> messages;

		auto end = m_InfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

		for (auto i = m_NextMessage; i < end; i++)
		{
			SIZE_T messageLength;

			ThrowOnError(m_InfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

			auto bytes = std::make_unique<byte[]>(messageLength);
			auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

			ThrowOnError(m_InfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, &messageLength));
			messages.emplace_back(message->pDescription);
		}

		return messages;
	}

	std::string GraphicsInfo::ThrowException(std::vector<std::string>& errorMessages)
	{
		for (const auto& message : errorMessages)
		{
			m_ErrorMessage += message;
			m_ErrorMessage.push_back('\n');
		}

		if (!m_ErrorMessage.empty())
			m_ErrorMessage.pop_back();

		GraphicsInfo::Release();

		return m_ErrorMessage;
	}

	void GraphicsInfo::Release()
	{
		if (!m_InfoQueue)
			m_InfoQueue->Release();
	}
}

