#pragma once
#include "Sibyl/Core/Window.h"
#include "Sibyl/Graphic/AbstractAPI/Core/Top/GraphicContext.h"
#include <GLFW/glfw3.h>

namespace SIByL
{
	class GLFWWindow :public Window
	{
	public:
		GLFWWindow(const WindowProps& props);
		virtual ~GLFWWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		virtual float GetHighDPI() override;

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override
		{
			m_Data.EventCallback = callback;
		}

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const override;
		static inline GLFWWindow* Get() { return Main; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		static GLFWWindow* Main;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
		std::unique_ptr<GraphicContext> m_OpenGLContext;
	};
}