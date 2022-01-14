export module Core.Input.GLFW;

import Core.Window;
import Core.Input;

namespace SIByL
{
	inline namespace Core
	{
		export class IInputGLFW : public IInput
		{
		public:
			IInputGLFW(IWindow* attached_window);

			virtual auto isKeyPressed(int keycode) noexcept -> bool override;
			virtual auto isMouseButtonPressed(int button) noexcept -> bool override;
			virtual auto getMousePosition(int button) noexcept -> std::pair<float, float> override;
			virtual auto getMouseX(int button) noexcept -> bool override;
			virtual auto getMouseY(int button) noexcept -> bool override;

		private:
			IWindow* attached_window;
		};

	}
}