#pragma once

#include <glm/glm.hpp>
#include "Sibyl/ECS/Asset/CustomAsset.h"

namespace SIByL
{
	class Material;
}

namespace SIByLEditor
{
	extern void DrawMaterial(const std::string& label, SIByL::Material& material);
	extern void DrawPipelineState(const std::string& label, SIByL::Material& material);
}

namespace SIByL
{
	class Shader;
	class Texture2D;
	class TextureCubemap;
	class ShaderConstantsBuffer;
	class ShaderResourcesBuffer;
	class ShaderConstantsDesc;
	class ShaderResourcesDesc;

	enum class AlphaState
	{
		Opaque,
		AlphaTest,
		AlphaBlend,
	};
	static std::string GetAlphaStateString(AlphaState alphaState)
	{
		switch (alphaState)
		{
		case SIByL::AlphaState::Opaque:
			return "Opaque";
			break;
		case SIByL::AlphaState::AlphaTest:
			return "AlphaTest";
			break;
		case SIByL::AlphaState::AlphaBlend:
			return "AlphaBlend";
			break;
		default:
			return "?";
			break;
		}
	}
	struct PipelineStateDesc
	{
		AlphaState alphaState;
	};

	class RenderTarget;
	class Material :public CustomAsset
	{
	public:
		void SetPass();

		////////////////////////////////////////////////////////////////////
		///					Parameter Setter / Getter					 ///
		void SetFloat(const std::string& name, const float& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMatrix4x4(const std::string& name, const glm::mat4& value);
		void SetTexture2D(const std::string& name, Ref<Texture2D> texture);
		void SetTexture2D(const std::string& name, RenderTarget* texture);
		void SetTextureCubemap(const std::string& name, Ref<TextureCubemap> texture);

		void GetFloat(const std::string& name, float& value);
		void GetFloat3(const std::string& name, glm::vec3& value);
		void GetFloat4(const std::string& name, glm::vec4& value);
		void GetMatrix4x4(const std::string& name, glm::mat4& value);

		float* PtrFloat(const std::string& name);
		float* PtrFloat3(const std::string& name);
		float* PtrFloat4(const std::string& name);
		float* PtrMatrix4x4(const std::string& name);

		void SetDirty();

		////////////////////////////////////////////////////////////////////
		///							Initializer							 ///
		Material() = default;
		Material(Ref<Shader> shader);
		void UseShader(Ref<Shader> shader);
		void OnDrawCall();

		////////////////////////////////////////////////////////////////////
		///							Fetcher								 ///
		ShaderConstantsDesc* GetConstantsDesc();
		ShaderResourcesDesc* GetResourcesDesc();
		ShaderResourcesDesc* GetCubeResourcesDesc();
		Ref<Shader> GetShaderUsed() { return m_Shader; }

		////////////////////////////////////////////////////////////////////
		///						Custom Asset							 ///
		virtual void SaveAsset() override;

	private:
		Ref<Shader> m_Shader = nullptr;
		Ref<ShaderConstantsBuffer> m_ConstantsBuffer = nullptr;
		Ref<ShaderResourcesBuffer> m_ResourcesBuffer = nullptr;
		Ref<ShaderResourcesBuffer> m_CubeResourcesBuffer = nullptr;
		PipelineStateDesc pipelineStateDesc;

		ShaderConstantsDesc* m_ConstantsDesc = nullptr;
		ShaderResourcesDesc* m_ResourcesDesc = nullptr;
		ShaderResourcesDesc* m_CubeResourcesDesc = nullptr;

		friend class DrawItem;
		friend class Camera;
		friend class FrameConstantsManager;
		friend void SIByLEditor::DrawMaterial(const std::string& label, SIByL::Material& material);
		friend void SIByLEditor::DrawPipelineState(const std::string& label, SIByL::Material& material);

	};

	class MaterialSerializer
	{
	public:
		MaterialSerializer(const Ref<Material>& material);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);

	private:
		Ref<Material> m_Material;
	};
}