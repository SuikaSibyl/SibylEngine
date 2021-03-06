#pragma once

#include "glm/glm.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include "Sibyl/ECS/Components/Components.h"

namespace SIByL
{
	class Shader;
	class Material;
	class Texture2D;
	class TriangleMesh;
	class LightComponent;
	class ShaderConstantItem;
	class MeshRendererComponent;
}

namespace SIByLEditor
{
	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columeWidth = 100);
	void DrawTriangleMeshSocket(const std::string& label, SIByL::MeshFilterComponent& mesh);
	void DrawMeshRendererMaterialSocket(const std::string& label, const std::string& passName, SIByL::MeshRendererComponent& meshRenderer, int i);
	void DrawLight(const std::string& label, SIByL::LightComponent& light);

	void DrawTexture2D(SIByL::Material& material, SIByL::ShaderConstantItem& item);

	void DrawShaderSlot(const std::string& label, SIByL::Ref<SIByL::Shader>* shader);

	void DrawMaterialSlot(const std::string& label);
	void DrawMaterial(const std::string& label, SIByL::Material& material);
	//void DrawMeshRenderer(const std::string& label, SIByL::)
}