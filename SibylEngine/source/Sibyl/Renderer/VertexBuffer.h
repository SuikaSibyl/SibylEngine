#pragma once

#include <iostream>
#include "Sibyl/Graphic/Geometry/Vertex.h"

namespace SIByL
{
	class VertexBuffer
	{
	public:
		enum class Type
		{
			Static,
			Dynamic,
			Stream,
		};

		static VertexBuffer* Create(float* vertices, uint32_t vCount, Type type = Type::Static);
		virtual void SetLayout(const VertexBufferLayout& layout) = 0;
		virtual const VertexBufferLayout& GetLayout() = 0;

	private:

	};
}