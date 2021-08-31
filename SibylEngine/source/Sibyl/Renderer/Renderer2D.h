#pragma once

#include "Sibyl/Graphic/Core/Camera.h"
#include "Sibyl/Graphic/Texture/Texture.h"

namespace SIByL
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(Ref<Camera> camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D> texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture);
	};
}