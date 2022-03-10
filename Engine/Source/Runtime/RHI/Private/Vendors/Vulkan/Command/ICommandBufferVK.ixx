module;
#include <cstdint>
#include <vulkan/vulkan.h>
export module RHI.ICommandBuffer.VK;
import RHI.ICommandBuffer;
import RHI.ICommandPool;
import RHI.ICommandPool.VK;
import RHI.ILogicalDevice;
import RHI.ILogicalDevice.VK;
import RHI.IEnum;
import RHI.IRenderPass;
import RHI.IRenderPass.VK;
import RHI.IFramebuffer;
import RHI.IFramebuffer.VK;
import RHI.IPipeline;
import RHI.IPipeline.VK;
import RHI.ISemaphore;
import RHI.ISemaphore.VK;
import RHI.IFence;
import RHI.IVertexBuffer;
import RHI.IIndexBuffer;
import RHI.IBuffer;
import RHI.IPipelineLayout;
import RHI.IDescriptorSet;
import RHI.IBarrier;
import RHI.ITexture;
import RHI.IStorageBuffer.VK;

namespace SIByL
{
	namespace RHI
	{
		export class ICommandBufferVK :public ICommandBuffer
		{
		public:
			ICommandBufferVK(ICommandPoolVK*, ILogicalDeviceVK*);
			ICommandBufferVK(ICommandBufferVK const&) = delete;
			ICommandBufferVK(ICommandBufferVK&&) = delete;
			virtual ~ICommandBufferVK() = default;
			// ICommandBuffer
			virtual auto reset() noexcept -> void override;
			virtual auto submit() noexcept -> void override;
			virtual auto submit(ISemaphore* wait, ISemaphore* signal, IFence* fence) noexcept -> void override;
			virtual auto beginRecording(CommandBufferUsageFlags flags = 0) noexcept -> void override;
			virtual auto endRecording() noexcept -> void override;
			virtual auto cmdBeginRenderPass(IRenderPass* render_pass, IFramebuffer* framebuffer) noexcept -> void override;
			virtual auto cmdEndRenderPass() noexcept -> void override;
			virtual auto cmdBindPipeline(IPipeline* pipeline) noexcept -> void override;
			virtual auto cmdBindComputePipeline(IPipeline* pipeline) noexcept -> void override;
			virtual auto cmdBindVertexBuffer(IVertexBuffer* buffer) noexcept -> void override;
			virtual auto cmdBindIndexBuffer(IIndexBuffer* buffer) noexcept -> void override;
			virtual auto cmdDraw(uint32_t const& vertex_count, uint32_t const& instance_count,
				uint32_t const& first_vertex, uint32_t const& first_instance) noexcept -> void override;
			virtual auto cmdCopyBuffer(IBuffer* src, IBuffer* dst, uint32_t const& src_offset, uint32_t const& dst_offset, uint32_t const& size) noexcept -> void override;
			virtual auto cmdDrawIndexed(uint32_t const& index_count, uint32_t const& instance_count,
				uint32_t const& first_index, uint32_t const& index_offset, uint32_t const& first_instance) noexcept -> void override;
			virtual auto cmdBindDescriptorSets(PipelineBintPoint point, IPipelineLayout* pipeline_layout, uint32_t const& idx_first_descset, 
				uint32_t const& count_sets_to_bind, IDescriptorSet** sets, uint32_t const&, uint32_t const*) noexcept -> void override;
			virtual auto cmdPipelineBarrier(IBarrier* barrier) noexcept -> void override;
			virtual auto cmdCopyBufferToImage(IBuffer* buffer, ITexture* image, IBufferImageCopy*) noexcept -> void override;
			virtual auto cmdDispatch(uint32_t const& x, uint32_t const& y, uint32_t const& z) noexcept -> void override;
			virtual auto cmdPushConstants(IPipelineLayout* pipeline_layout, ShaderStage stage, size_t size, void* data) noexcept -> void override;
			virtual auto cmdDrawIndexedIndirect(IStorageBuffer* storageBuffer, uint32_t const& indiret_offset, uint32_t const& draw_count, uint32_t const& draw_stride) noexcept -> void override;

		private:
			auto createVkCommandBuffer() noexcept -> void;
			ILogicalDeviceVK* logicalDevice;
			ICommandPoolVK* commandPool;
			VkCommandBuffer commandBuffer;
		};
	}
}
