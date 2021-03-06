#include "SIByLpch.h"
#include "SceneSerializer.h"

#include "yaml-cpp/yaml.h"
#include "yaml-cpp/node/node.h"
#include "Sibyl/ECS/Core/SerializeUtility.h"

#include "Sibyl/ECS/Core/Entity.h"
#include "Sibyl/ECS/Components/Components.h"

#include "Sibyl/ECS/Asset/AssetUtility.h"
#include "Sibyl/Graphic/Core/Geometry/MeshLoader.h"
#include "Sibyl/Graphic/AbstractAPI/Core/Top/Material.h"

namespace SIByL
{
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Scene(scene)
	{

	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "124531623461";

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& transform = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << transform.Translation;
			out << YAML::Key << "EulerAngles" << YAML::Value << transform.EulerAngles;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshFilterComponent>())
		{
			out << YAML::Key << "MeshFilterComponent";
			out << YAML::BeginMap;

			auto& meshFilter = entity.GetComponent<MeshFilterComponent>();
			out << YAML::Key << "Mesh" << YAML::Value << meshFilter.Mesh;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshRendererComponent>())
		{
			out << YAML::Key << "MeshRendererComponent";
			out << YAML::BeginMap;

			auto& meshRenderer = entity.GetComponent<MeshRendererComponent>();

			out << YAML::Key << "SubmeshNum" << YAML::Value << meshRenderer.SubmeshNum;

			out << YAML::Key << "Passes" << YAML::Value << YAML::BeginSeq;

			for (auto& iter : meshRenderer.Materials)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Pass" << YAML::Value << iter.first;
				out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;

				{
					int i = 0;
					for (auto subMaterial : meshRenderer.Materials[iter.first])
					{
						out << YAML::BeginMap;
						out << YAML::Key << "Material" << YAML::Value << i++;

						out << YAML::Key << "INFO";
						out << YAML::BeginMap;
						out << YAML::Key << "Path" << YAML::Value << ((subMaterial == nullptr) ? "EMPTY" : subMaterial->GetSavePath());
						out << YAML::EndMap;

						out << YAML::EndMap;
					}
				}
				out << YAML::EndSeq;
				out << YAML::EndMap;
			}

			out << YAML::EndSeq;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<LightComponent>())
		{
			out << YAML::Key << "LightComponent";
			out << YAML::BeginMap;

			auto& lightFilter = entity.GetComponent<LightComponent>();
			out << YAML::Key << "Type" << YAML::Value << (unsigned int)lightFilter.m_Type;
			out << YAML::Key << "Position" << YAML::Value << lightFilter.m_Position;
			out << YAML::Key << "Direction" << YAML::Value << lightFilter.m_Direction;
			out << YAML::Key << "Color" << YAML::Value << lightFilter.m_Color;
			out << YAML::Key << "Intensity" << YAML::Value << lightFilter.m_Intensity;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not Implemented
		//SIByL_CORE_ASSERT(false);
		return;
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::NodeAoS data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		SIByL_CORE_ASSERT("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				// Tag Component
				// -----------------------------------------------
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				SIByL_CORE_TRACE("Deserialized entity with ID = {0}, NAME = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				// Transform Component
				// -----------------------------------------------
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.EulerAngles = transformComponent["EulerAngles"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				// MeshFilter Component
				// -----------------------------------------------
				auto meshFilterComponent = entity["MeshFilterComponent"];
				if (meshFilterComponent)
				{
					auto& tc = deserializedEntity.AddComponent<MeshFilterComponent>();
					tc.Path = meshFilterComponent["Mesh"].as<std::string>();

					SIByL::MeshLoader meshLoader(tc.Path, SIByL::VertexBufferLayout::StandardVertexBufferLayout);
					tc.Mesh = meshLoader.GetTriangleMesh();
				}

				// MeshRenderer Component
				// -----------------------------------------------
				auto meshRendererComponent = entity["MeshRendererComponent"];
				if (meshRendererComponent)
				{
					auto& mrc = deserializedEntity.AddComponent<MeshRendererComponent>();
					unsigned int SubmeshNum = meshRendererComponent["SubmeshNum"].as<unsigned int>();
					mrc.SetMaterialNums(SubmeshNum);

					YAML::NodeAoS passes = meshRendererComponent["Passes"];
					if (passes)
					{
						int passIdx = 0;
						for (auto pass : passes)
						{
							std::string PassName = pass["Pass"].as<std::string>();
							YAML::NodeAoS materials = pass["Materials"];
							if (materials)
							{
								std::vector<Ref<Material>>& mats = mrc.GetPassMaterials(PassName);
								int materialInd = 0;
								for (auto material : materials)
								{
									auto info = material["INFO"];
									std::string relativePathString = info["Path"].as<std::string>();
									if (relativePathString != "NONE")
									{
										if (relativePathString == "EMPTY")
										{
											mats[materialInd++] = nullptr;
										}
										else
										{
											mats[materialInd++] = GetAssetByPath<Material>(relativePathString);
										}
									}
								}
							}

						}
					}
				}


				// MeshFilter Component
				// -----------------------------------------------
				auto lightComponent = entity["LightComponent"];
				if (lightComponent)
				{
					auto& lc = deserializedEntity.AddComponent<LightComponent>();
					lc.m_Type = (LightType)lightComponent["Type"].as<unsigned int>();
					lc.m_Position = lightComponent["Position"].as<glm::vec3>();
					lc.m_Direction = lightComponent["Direction"].as<glm::vec3>();
					lc.m_Color = lightComponent["Color"].as<glm::vec3>();
					lc.m_Intensity = lightComponent["Intensity"].as<float>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not Implemented
		//SIByL_CORE_ASSERT(false);
		return false;
	}

}