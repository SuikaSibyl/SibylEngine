#pragma once

#include "Core.h"
#include "Window.h"
#include "Sibyl/Core/Events/ApplicationEvent.h"
#include "Sibyl/Core/Events/KeyEvent.h"
#include "Sibyl/Core/Events/MouseEvent.h"
#include "Sibyl/Core/LayerStack.h"
#include "Sibyl/Core/FrameTimer.h"
#include "Sibyl/Graphic/AbstractAPI/Core/Top/GraphicContext.h"
#include "Sibyl/ImGui/ImGuiLayer.h"
#include "Sibyl/Graphic/Core/Geometry/TriangleMesh.h"

namespace SIByL
{
	class SIByL_API Application
	{
	public:
		Application(const std::string& name = "SIByL Enigne");
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
		void OnAwake();
		void OnDraw();
		void OnResourceDestroy();
		void DrawImGui();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		LayerStack& GetLayerStack() { return m_LayerStack; }
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
		inline FrameTimer* GetFrameTimer() { return m_FrameTimer.get(); }

	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		Ref<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<FrameTimer> m_FrameTimer;

		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;

		bool m_IsMinimized = false;
	};

	// Defined in client
	Application* CreateApplication();
}