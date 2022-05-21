#pragma once

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <wrl.h>

static constexpr uint32_t s_BufferCount = 3;

namespace nino
{
	enum class PrimitiveType
	{
		TRIANGLE,
		QUAD,
	};
}