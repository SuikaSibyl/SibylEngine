module;
#include <cstdint>
#include <vulkan/vulkan.h>
export module RHI.IMemoryBarrierVK;
import RHI.IMemoryBarrier;
import RHI.IEnum;
import RHI.ICommandQueue;
import RHI.ITexture;

namespace SIByL
{
	namespace RHI
	{
        export class IImageMemoryBarrierVK
        {
        public:
            IImageMemoryBarrierVK(ImageMemoryBarrierDesc const&);
            virtual ~IImageMemoryBarrierVK() = default;
            auto getVkImageMemoryBarrier() noexcept -> VkImageMemoryBarrier*;

        private:
            VkImageMemoryBarrier barrier{};
        };
	}
}