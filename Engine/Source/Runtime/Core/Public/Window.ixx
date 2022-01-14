module;
#include <cstdint>
#include <functional>
export module Core.Window;
import Core.Enums;
import Core.Event;
import Core.Input;
import Core.Layer;

namespace SIByL
{
	inline namespace Core
	{
		export using EventCallbackFn = std::function<void(Event&)>;

		export class IWindow
		{
		public:
			virtual ~IWindow() = default;

			virtual auto onUpdate() noexcept -> void = 0;
			virtual auto getWidth() const noexcept -> unsigned int = 0;
			virtual auto getHeight() const noexcept -> unsigned int = 0;
			virtual auto getHighDPI() const noexcept -> float = 0;
			virtual auto getNativeWindow() const noexcept -> void* = 0;
			virtual auto setVSync(bool enabled) noexcept -> void = 0;
			virtual auto isVSync() const noexcept -> bool = 0;
			virtual auto setEventCallback(const EventCallbackFn& callback) noexcept -> void = 0;
			virtual auto getInput() const noexcept -> IInput* = 0;
		};

		export class WindowLayer: public ILayer
		{
		public:
			WindowLayer(
				EWindowVendor vendor,
				EventCallbackFn event_callback,
				uint32_t const& width,
				uint32_t const& height,
				std::string_view name);
			
			~WindowLayer()
			{
				window.reset(nullptr);
			}

			virtual void onUpdate() override;
			virtual void onShutdown() override;
			IWindow* getWindow();

		private:
			std::unique_ptr<IWindow> window;
		};
	}
}