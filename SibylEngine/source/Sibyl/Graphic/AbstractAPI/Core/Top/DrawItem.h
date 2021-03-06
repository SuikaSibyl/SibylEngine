#pragma once

#include "glm/glm.hpp"

namespace SIByL
{
	struct SubMesh;
	class TriangleMesh;
	class Material;
	class ShaderConstantsBuffer;

	class DrawItem
	{
	public:
		DrawItem();
		DrawItem(Ref<TriangleMesh> mesh);
		DrawItem(Ref<TriangleMesh> mesh, SubMesh* submesh);
		~DrawItem();

		void SetObjectMatrix(const glm::mat4& transform);
		void OnDrawCall();

		Ref<Material> m_Material;
		Ref<TriangleMesh> m_Mesh;
		SubMesh* m_SubMesh;
		Ref<ShaderConstantsBuffer> m_ConstantsBuffer = nullptr;
	};
}