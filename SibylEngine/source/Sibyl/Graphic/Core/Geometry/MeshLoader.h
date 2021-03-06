#pragma once

class aiNode;
class aiScene;
class aiMesh;

#include <Sibyl/Basic/Asset/SCacheAsset.h>
#include <Sibyl/Graphic/AbstractAPI/Core/Middle/VertexBuffer.h>

namespace SIByL
{
	class TriangleMesh;

	struct MeshData
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		unsigned int vNum;
		unsigned int iNum;

		MeshData() = default;
		MeshData(const std::vector<float>& v,
			const std::vector<unsigned int>& i,
			const unsigned int& vn, const unsigned int& in)
			:vertices(v), indices(i), vNum(vn), iNum(in) {}
	};

	struct MeshDataU32
	{
		std::vector<float> vertices;
		std::vector<uint32_t> indices;

		uint32_t vNum;
		uint32_t iNum;

		MeshDataU32() = default;
		MeshDataU32(const std::vector<float>& v,
			const std::vector<uint32_t>& i,
			const uint32_t& vn, const uint32_t& in)
			:vertices(v), indices(i), vNum(vn), iNum(in) {}
	};

	class SMeshCacheAsset :public SCacheAsset
	{
	public:
		SMeshCacheAsset(const std::string& assetpath);
		std::vector<MeshData> m_Meshes;

		virtual void LoadDataToBuffers() override;
		virtual void LoadDataFromBuffers() override;

	};

	class MeshLoader
	{
	public:
		MeshLoader(const std::string& path, const VertexBufferLayout& layout);
		Ref<TriangleMesh> GetTriangleMesh();
		SMeshCacheAsset* GetSMeshCache() { return &m_MeshCacheAsset; }

	private:
		void LoadFile(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);
		VertexBufferLayout m_Layout;
		SMeshCacheAsset m_MeshCacheAsset;
		std::string m_Directory;
		std::string m_Path;
	};
}