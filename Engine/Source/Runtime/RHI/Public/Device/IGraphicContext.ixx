module;
#include <cstdint>
export module RHI.GraphicContext;
import Core.SObject;
import Core.Window;
import RHI.IEnum;

namespace SIByL
{
	namespace RHI
	{
		// The context generally allows you to access the API's inner classes.
		// ╭──────────────┬──────────────────╮
		// │  Vulkan	  │   vk::Instance   │
		// │  DirectX 12  │   IDXGIFactory   │
		// │  OpenGL      │   Varies by OS   │
		// ╰──────────────┴──────────────────╯

		export enum struct GraphicContextExtensionFlagBits
		{
			DEBUG_UTILS = 0x00000001,
			QUERY	    = 0x00000002,
			MESH_SHADER = 0x00000004,
			SHADER_INT8 = 0x00000008,
			FRAGMENT_BARYCENTRIC = 0x00000010,
			SAMPLER_FILTER_MIN_MAX = 0x00000020,
		};
		export using GraphicContextExtensionFlags = uint32_t;

		export class IGraphicContext :public SObject
		{
		public:
			virtual ~IGraphicContext() = default;

			virtual auto attachWindow(IWindow* window) noexcept -> void = 0;
			auto setAPI(API const& _api) noexcept -> void { api = _api; }
			auto getAPI() noexcept -> API { return api; }

			auto getExtensions() noexcept -> GraphicContextExtensionFlags { return extensions; }

		protected:
			API api;
			GraphicContextExtensionFlags extensions;
		};
	}
}