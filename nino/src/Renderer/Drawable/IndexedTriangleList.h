#pragma once

#include <vector>
#include <DirectXMath.h>

namespace nino
{
	template<class T>
	class IndexedTriangleList
	{
	public:
		IndexedTriangleList() = default;
		IndexedTriangleList(std::vector<T> vertsIn, std::vector<unsigned short> indicesIn)
			: 
			Vertices(std::move(vertsIn)),
			Indices(std::move(indicesIn))
		{}

	public:
		std::vector<T> Vertices;
		std::vector<unsigned short> Indices;
	};
}