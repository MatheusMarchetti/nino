#pragma once

#include "Core/Core.h"

#include "Renderer/Drawable/DrawableBase.h"
#include "Renderer/Drawable/IndexedTriangleList.h"

namespace nino
{
	class Cube : public DrawableBase<Cube>
	{
	public:
		Cube();

		template <class T>
		static IndexedTriangleList<T> Make()
		{
			constexpr float side = 1.0f / 2.0f;

			std::vector<T> vertices(24);
			vertices[0].Position = { -side, -side, -side};
			vertices[1].Position = {  side, -side, -side};
			vertices[2].Position = { -side,  side, -side};
			vertices[3].Position = {  side,  side, -side};
			vertices[4].Position = {  side, -side, -side};
			vertices[5].Position = {  side, -side,  side};
			vertices[6].Position = {  side,  side, -side};
			vertices[7].Position = {  side,  side,  side};
			vertices[8].Position = { -side, -side,  side};
			vertices[9].Position = { -side, -side, -side};
			vertices[10].Position = {-side,  side,  side};
			vertices[11].Position = {-side,  side, -side};
			vertices[12].Position = { side, -side,  side};
			vertices[13].Position = {-side, -side,  side};
			vertices[14].Position = { side,  side,  side};
			vertices[15].Position = {-side,  side,  side};
			vertices[16].Position = {-side,  side, -side};
			vertices[17].Position = { side,  side, -side};
			vertices[18].Position = {-side,  side,  side};
			vertices[19].Position = { side,  side,  side};
			vertices[20].Position = {-side, -side,  side};
			vertices[21].Position = { side, -side,  side};
			vertices[22].Position = {-side, -side, -side};
			vertices[23].Position = { side, -side, -side};

			vertices[0].TextureCoords = {0.0f, 1.0f};
			vertices[1].TextureCoords = {1.0f, 1.0f};
			vertices[2].TextureCoords = {0.0f, 0.0f};
			vertices[3].TextureCoords = {1.0f, 0.0f};
			vertices[4].TextureCoords = {0.0f, 1.0f};
			vertices[5].TextureCoords = {1.0f, 1.0f};
			vertices[6].TextureCoords = {0.0f, 0.0f};
			vertices[7].TextureCoords = {1.0f, 0.0f};
			vertices[8].TextureCoords = {0.0f, 1.0f};
			vertices[9].TextureCoords = {1.0f, 1.0f};
			vertices[10].TextureCoords = {0.0f, 0.0f};
			vertices[11].TextureCoords = {1.0f, 0.0f};
			vertices[12].TextureCoords = {0.0f, 1.0f};
			vertices[13].TextureCoords = {1.0f, 1.0f};
			vertices[14].TextureCoords = {0.0f, 0.0f};
			vertices[15].TextureCoords = {1.0f, 0.0f};
			vertices[16].TextureCoords = {0.0f, 1.0f};
			vertices[17].TextureCoords = {1.0f, 1.0f};
			vertices[18].TextureCoords = {0.0f, 0.0f};
			vertices[19].TextureCoords = {1.0f, 0.0f};
			vertices[20].TextureCoords = {0.0f, 1.0f};
			vertices[21].TextureCoords = {1.0f, 1.0f};
			vertices[22].TextureCoords = {0.0f, 0.0f};
			vertices[23].TextureCoords = {1.0f, 0.0f};

			return
			{
				std::move(vertices),
				{
					0,2,1,    2,3,1,
					4,6,5,    6,7,5,
					8,10,9,   10,11,9,
					12,14,13, 14,15,13,
					16,18,17, 18,19,17,
					20,22,21, 22,23,21
				}
			};
		}
	};
}